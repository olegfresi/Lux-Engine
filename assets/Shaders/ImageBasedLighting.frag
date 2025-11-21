#version 410 core
#define PI 3.1415926535897932384626433832795

vec3 ImportanceSampleGGX(vec2 Xi, float Roughness, vec3 N)
{
    float a = Roughness * Roughness;
    float Phi = 2 * PI * Xi.x;
    float CosTheta = sqrt((1 - Xi.y) / (1 + (a*a - 1) * Xi.y));
    float SinTheta = sqrt(1 - CosTheta * CosTheta);
    vec3 H;
    H.x = SinTheta * cos(Phi);
    H.y = SinTheta * sin(Phi);
    H.z = CosTheta;

    vec3 Up = abs(N.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
    vec3 TangentX = normalize(cross(Up, N));
    vec3 TangentY = normalize(cross(N, TangentX));

    return TangentX * H.x + TangentY * H.y + N * H.z;
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i) / float(N), float(i) * 0.618033988749895 - floor(float(i) * 0.618033988749895));
}

float G_Smith(float Roughness, float NoV, float NoL)
{
    float a = Roughness * Roughness;
    float k = a / 2.0;
    float G1 = NoV / (NoV * (1 - k) + k);
    float G2 = NoL / (NoL * (1 - k) + k);
    return G1 * G2;
}

vec3 PrefilterEnvMap( float Roughness, vec3 R )
{
    vec3 N = R;
    vec3 V = R;
    vec3 PrefilteredColor = vec3(0.0);
    const uint NumSamples = 1024;
    for( uint i = 0; i < NumSamples; i++ )
    {
        vec2 Xi = Hammersley( i, NumSamples );
        vec3 H = ImportanceSampleGGX( Xi, Roughness, N );
        vec3 L = 2 * dot( V, H ) * H - V;
        float NoL = clamp( dot( N, L ), 0.0, 1.0 );
        if( NoL > 0 )
        {
            PrefilteredColor += EnvMap.SampleLevel( EnvMapSampler , L, 0 ).rgb * NoL;
            TotalWeight += NoL;
        }
    }
    return PrefilteredColor / TotalWeight;
}

vec3 SpecularIBL(vec3 SpecularColor, float Roughness, vec3 N, vec3 V)
{
    vec3 SpecularLighting = vec3(0.0);
    const uint NumSamples = 1024;

    for(uint i = 0; i < NumSamples; i++)
    {
        vec2 Xi = Hammersley(i, NumSamples);
        vec3 H = ImportanceSampleGGX(Xi, Roughness, N);
        vec3 L = 2 * dot(V, H) * H - V;

        float NoV = clamp(dot(N, V), 0.0, 1.0);
        float NoL = clamp(dot(N, L), 0.0, 1.0);
        float NoH = clamp(dot(N, H), 0.0, 1.0);
        float VoH = clamp(dot(V, H), 0.0, 1.0);

        if(NoL > 0)
        {
            vec3 SampleColor = EnvMap.SampleLevel(EnvMapSampler, L, 0).rgb;

            float G = G_Smith(Roughness, NoV, NoL);
            float Fc = pow(1 - VoH, 5);
            vec3 F = (1 - Fc) * SpecularColor + Fc;

            SpecularLighting += SampleColor * F * G * VoH / (NoH * NoV);
        }
    }

    return SpecularLighting / NumSamples;
}

vec2 IntegrateBRDF( float Roughness, float NoV )
{
    vec3 V;
    V.x = sqrt( 1.0f - NoV * NoV ); // sin
    V.y = 0;
    V.z = NoV; // cos
    float A = 0;
    float B = 0;
    const uint NumSamples = 1024;
    for( uint i = 0; i < NumSamples; i++ )
    {
        vec2 Xi = Hammersley( i, NumSamples );
        vec3 H = ImportanceSampleGGX( Xi, Roughness, N );
        vec3 L = 2 * dot( V, H ) * H - V;
        float NoL = clamp( L.z, 0.0, 1.0 );
        float NoH = clamp( H.z, 0.0, 1.0 );
        float VoH = clamp( dot( V, H ), 0.0, 1.0 );
        if( NoL > 0 )
        {
            float G = G_Smith( Roughness, NoV, NoL );
            float G_Vis = G * VoH / (NoH * NoV);
            float Fc = pow( 1 - VoH, 5 );
            A += (1 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    return vec2( A, B ) / NumSamples;
}

vec3 ApproximateSpecularIBL(vec3 SpecularColor , float Roughness, vec3 N, vec3 V )
{
    float NoV = clamp( dot( N, V ), 0.0, 1.0 );
    vec3 R = 2 * dot( V, N ) * N - V;
    vec3 PrefilteredColor = PrefilterEnvMap( Roughness, R );
    vec3 EnvBRDF = IntegrateBRDF( Roughness, NoV );
    return PrefilteredColor * ( SpecularColor * EnvBRDF.x + EnvBRDF.y );
}