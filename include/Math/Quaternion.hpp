/*
 * Project: TestProject
 * File: Quaternion.hpp
 * Author: olegfresi
 * Created: 26/02/25 17:38
 *
 * Copyright © 2025 olegfresi
 *
 * Licensed under the MIT License. You may obtain a copy of the License at:
 *
 *     https://opensource.org/licenses/MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include <cmath>
#include <type_traits>
#include <array>
#include <complex>
#include <iterator>
#include <numbers>
#include <cassert>

namespace lux::math
{
#define IS_ITERATOR(X) typename std::enable_if<!std::is_same<typename std::iterator_traits<X>::value_type, void>::value>::type* =nullptr
#define IS_CONVERTIBLE(FROM, TO) typename std::enable_if<std::is_convertible<FROM,TO>::value>::type* =nullptr

    template<typename T1, typename T2>
    bool IsScalarZero(T1 x, T2 eps = 0)
    {
        using T = std::common_type_t<T1,T2>;
        T xx = static_cast<T>(x);
        T ee = static_cast<T>(eps);
        return std::abs(xx) <= ee;
    }

    template<typename T1, typename T2, typename T3>
    bool IsNearlyEqual(T1 x, T2 y, T3 eps = 0)
    {
        using T =  std::common_type_t<T1,T2,T3>;
        T xx = static_cast<T>(x);
        T yy = static_cast<T>(y);
        T ee = static_cast<T>(eps);

        if (xx == 0)
            return IsScalarZero(yy, ee);
        if (yy == 0)
            return IsScalarZero(xx, ee);

        return IsScalarZero((xx - yy) / std::min(xx, yy), ee);
    }

    template<typename T = double>
    class Quaternion
    {
    public:
        /**
         * The type of each component of the 4 components of a Quaternion.
         * See below for allowed types.
         */
        using value_type = T;

        /**
         * For now, the types that we can handle are restricted, to make sure
         * that we are not asked to handle a type that would lead to bugs
         * (e.g. unsigned T when we have signed operations, or complex T).
         * boost::rational is technically OK, and I verified in tests,
         * but I don't want to depend on boost in this header, so we'll allow
         * boost::rational if there is demand for it.
         *
         * WARNING: for bool and the integers, some operations won't compile right now,
         * in particular the transcendental functions.
         */
        static_assert(std::is_same<T, bool>()
                      || std::is_same<T, int>()
                      || std::is_same<T, long>()
                      || std::is_same<T, long long>()
                      || std::is_same<T, float>()
                      || std::is_same<T, double>()
                      || std::is_same<T, long double>(),
                      "Invalid scalar type for Quaternion");

        /**
        * Construct a Quaternion from at most 4 components of type T.
        * Specifying only a != 0 makes the Quaternion a real.
        * Specifying only a != and b != 0 makes the Quaternion an ordinary complex number.
        */
        explicit Quaternion(T a = 0, T b = 0, T c = 0, T d = 0)
                : _a(a), _b(b), _c(c), _d(d) { }

        /**
         * Construct a Quaternion from at most 4 components of type T1.
         * Specifying only a != 0 makes the Quaternion a real.
         * Specifying only a != and b != 0 makes the Quaternion an ordinary complex number.
         * NOTE: IS_CONVERTIBLE to avoid ambiguity with constructor from iterator.
         */
        template<typename T1, IS_CONVERTIBLE(T1,T)>
        explicit Quaternion(T1 a = 0, T1 b = 0, T1 c = 0, T1 d = 0)
                : _a(a), _b(b), _c(c), _d(d) { }

        /**
         * Construct a Quaternion from 1 or 2 std::complex<T>.
         */
        template<typename T1>
        explicit Quaternion(const std::complex<T1>& x, const std::complex<T1>& y = std::complex<T1>(0, 0))
                : _a(x.real()), _b(x.imag()), _c(y.real()), _d(y.imag()) { }

        /**
         * Construct from an iterator to a range of 4 elements.
         * The iterator is dereferenced exactly 4 times, and advanced (++)
         * exactly 3 times.
         */
        template<typename It, IS_ITERATOR(It)>
        explicit Quaternion(It it) : _a(*it), _b(*++it), _c(*++it), _d(*++it) { }

        /**
         * Copy constructor, from a Quaternion with another value type.
         */
        template<typename T1>
        explicit Quaternion(const Quaternion<T1>& y) : _a(y.a()), _b(y.b()), _c(y.c()), _d(y.d()) { }

        /**
         * Assignment operator, from a Quaternion with another value type.
         */
        template<typename T1>
        Quaternion& operator=(const Quaternion<T1>& other)
        {
            _a = other.a();
            _b = other.b();
            _c = other.c();
            _d = other.d();

            return *this;
        }

        /**
         * Accessors for all 4 components of the Quaternion.
         */
        T A() const { return _a; }
        T B() const { return _b; }
        T C() const { return _c; }
        T D() const { return _d; }

        /**
         * The complex components of this Quaternion.
         */
        std::complex<T> C1() const { return {_a, _b}; }
        std::complex<T> C2() const { return {_c, _d}; }

        /**
         * Ordered list form.
         */
        std::array<T, 4> ToArray() const
        {
            return {{_a, _b, _c, _d}};
        }

        /**
         * The real part of this Quaternion.
         */
        T Real() const { return _a; }

        /**
         * The "unreal" part of this Quaternion, which is a Quaternion itself.
         */
        Quaternion Unreal() const { return {0, _b, _c, _d}; }

        /**
         * The square of the norm of the Quaternion.
         * (The square is sometimes useful, and it avoids paying for a sqrt).
         */
        T NormSquared() const
        {
            return _a * _a + _b * _b + _c * _c + _d * _d;
        }

        /**
         * The norm of the Quaternion (the l2 norm).
         */
        T Abs() const
        {
            return std::sqrt(NormSquared());
        }

        /**
         * The L2 norm of the "unreal" components of the Quaternion,
         * comes back often in computations.
         */
        T UnrealNormSquared() const
        {
            return _b * _b + _c * _c + _d * _d;
        }

        void NormalizeReal()
        {
            _a = sqrt((T)1.0 - UnrealNormSquared());
        }

        /**
         * Return true if this Quaternion is zero, false otherwise.
         */
        template <typename T1 = T>
        bool IsZero(T1 eps = 0) const
        {
            return IsScalarZero(_a, eps)
                   && IsScalarZero(_b, eps)
                   && IsScalarZero(_c, eps)
                   && IsScalarZero(_d, eps);
        }

        /**
         * Return true if this Quaternion is not zero, false otherwise.
         */
        template <typename T1 =T>
        bool IsNonZero(T1 eps = 0) const
        {
            return !IsZero(eps);
        }

        /**
         * Return true if any component of this quaternion is nan, false otherwise.
         * TODO: use "isnan" instead of "is_nan" to keep consistent?
         */
        bool IsNan() const
        {
            return std::isnan(_a) || std::isnan(_b) || std::isnan(_c) || std::isnan(_d);
        }

        /**
        * Return true if any component of this quaternion is inf, false otherwise.
        */
        bool IsInf() const
        {
            return std::isinf(_a) || std::isinf(_b) || std::isinf(_c) || std::isinf(_d);
        }

        /**
        * Return true if all the components of this quaternion are finite, false otherwise.
        */
        bool IsFinite() const
        {
            return std::isfinite(_a) && std::isfinite(_b) && std::isfinite(_c) && std::isfinite(_d);
        }

        /**
         * Return true if this Quaternion has norm 1, false otherwise.
         */
        template<typename T1 = T>
        bool IsUnit(T1 eps = 0) const
        {
            return IsScalarZero(NormSquared() - T(1), eps);
        }

        /**
         * Return true if this Quaternion is real, false otherwise.
         */
        template<typename T1 =T>
        bool IsReal(T1 eps = 0) const
        {
            return IsScalarZero(_b, eps)
                   && IsScalarZero(_c, eps)
                   && IsScalarZero(_d, eps);
        }

        /**
         * Return true if this Quaternion is complex, false otherwise.
         */
        template<typename T1 =T>
        bool IsComplex(T1 eps = 0) const
        {
            return IsScalarZero(_c, eps) && IsScalarZero(_d, eps);
        }

        /**
         * Return true if this Quaternion is real, false otherwise.
         */
        template<typename T1 =T>
        bool IsUnreal(T1 eps = 0) const
        {
            return IsScalarZero(_a, eps)
                   && !(IsScalarZero(_b, eps)
                        && IsScalarZero(_c, eps)
                        && IsScalarZero(_d, eps));
        }

        Quaternion operator+() const
        {
            return *this;
        }

        Quaternion operator-() const
        {
            return {-_a, -_b, -_c, -_d};
        }

        Quaternion operator+=(T y)
        {
            _a += y;
            return *this;
        }

        Quaternion operator-=(T y)
        {
            _a -= y;
            return *this;
        }

        Quaternion operator*=(T k)
        {
            _a = k * _a;
            _b = k * _b;
            _c = k * _c;
            _d = k * _d;

            return *this;
        }

        Quaternion operator/=(T k)
        {
            _a /= k;
            _b /= k;
            _c /= k;
            _d /= k;

            return *this;
        }

        template<typename T1>
        Quaternion operator+=(const std::complex<T1>& y)
        {
            _a += y.real();
            _b += y.imag();

            return *this;
        }

        template<typename T1>
        Quaternion operator-=(const std::complex<T1>& y)
        {
            _a -= y.real();
            _b -= y.imag();

            return *this;
        }

        template<typename T1>
        Quaternion operator*=(const std::complex<T1>& y)
        {

            T at = _a * y.real() - _b * y.imag();
            T bt = _a * y.imag() + _b * y.real();
            T ct = _c * y.real() + _d * y.imag();
            T dt = -_c * y.imag() + _d * y.real();

            _a = at;
            _b = bt;
            _c = ct;
            _d = dt;

            return *this;
        }

        template<typename T1>
        Quaternion operator/=(const std::complex<T1>& y)
        {

            T n2 = y.real() * y.real() + y.imag() * y.imag();
            T at = _a * y.real() + _b * y.imag();
            T bt = -_a * y.imag() + _b * y.real();
            T ct = _c * y.real() - _d * y.imag();
            T dt = _c * y.imag() + _d * y.real();

            _a = at / n2;
            _b = bt / n2;
            _c = ct / n2;
            _d = dt / n2;

            return *this;
        }

        template<typename T1>
        Quaternion operator+=(const Quaternion<T1>& y)
        {
            _a += y.a();
            _b += y.b();
            _c += y.c();
            _d += y.d();

            return *this;
        }

        template<typename T1>
        Quaternion operator-=(const Quaternion<T1>& y)
        {
            _a -= y._a;
            _b -= y._b;
            _c -= y._c;
            _d -= y._d;

            return *this;
        }

        /**
         * Unary multiplication.
         * 28 operations
         */
        template<typename T1>
        Quaternion operator*=(const Quaternion<T1>& y) {

            T at = _a * y.a() - _b * y.b() - _c * y.c() - _d * y.d();
            T bt = _a * y.b() + _b * y.a() + _c * y.d() - _d * y.c();
            T ct = _a * y.c() - _b * y.d() + _c * y.a() + _d * y.b();
            T dt = _a * y.d() + _b * y.c() - _c * y.b() + _d * y.a();

            _a = at;
            _b = bt;
            _c = ct;
            _d = dt;

            return *this;
        }

        /**
         * Unary division with other Quaternion.
         *
         * Warning: if the norm of y is zero, the result is
         * 4 NaNs, but maybe it should be inf.
         */
        template<typename T1>
        Quaternion operator/=(const Quaternion<T1>& y)
        {

            T n2 = y.NormSquared();

            T at = _a * y.a() + _b * y.b() + _c * y.c() + _d * y.d();
            T bt = -_a * y.b() + _b * y.a() - _c * y.d() + _d * y.c();
            T ct = -_a * y.c() + _b * y.d() + _c * y.a() - _d * y.b();
            T dt = -_a * y.d() - _b * y.c() + _c * y.b() + _d * y.a();

            _a = at / n2;
            _b = bt / n2;
            _c = ct / n2;
            _d = dt / n2;

            return *this;
        }

    private:
        T _a, _b, _c, _d;
    };


    using Qf = Quaternion<float>;
    const Qf Qf_0;
    const Qf Qf_1(1);
    const Qf Qf_i(0, 1);
    const Qf Qf_j(0, 0, 1);
    const Qf Qf_k(0, 0, 0, 1);


    using Qd = Quaternion<double>;
    const Qd Qd_0;
    const Qd Qd_1(1);
    const Qd Qd_i(0, 1);
    const Qd Qd_j(0, 0, 1);
    const Qd Qd_k(0, 0, 0, 1);


    using Qld = Quaternion<long double>;
    const Qld Qld_0;
    const Qld Qld_1(1);
    const Qld Qld_i(0, 1);
    const Qld Qld_j(0, 0, 1);
    const Qld Qld_k(0, 0, 0, 1);

    template<typename T>
    Quaternion<T> Spherical(T rho, T theta, T phi1, T phi2)
    {
        T d = std::sin(phi2);
        T cr = std::cos(phi2);
        T c = cr * std::sin(phi1);
        cr *= std::cos(phi1);
        T b = cr * std::sin(theta);
        T a = cr * std::cos(theta);

        return {rho * a, rho * b, rho * c, rho * d};
    }

    template<typename T>
    Quaternion<T> Semipolar(T rho, T alpha, T theta1, T theta2)
    {
        T ca = std::cos(alpha);
        T sa = std::sin(alpha);
        T a = ca * std::cos(theta1);
        T b = ca * std::sin(theta1);
        T c = sa * std::cos(theta2);
        T d = sa * std::sin(theta2);

        return {rho * a, rho * b, rho * c, rho * d};
    }

    template<typename T>
    Quaternion<T> Multipolar(T rho1, T theta1, T rho2, T theta2)
    {
        T a = rho1 * std::cos(theta1);
        T b = rho1 * std::sin(theta1);
        T c = rho2 * std::cos(theta2);
        T d = rho2 * std::sin(theta2);

        return {a, b, c, d};
    }

    template<typename T>
    Quaternion<T> Cylindrospherical(T t, T radius, T longitude, T latitude)
    {
        T cl = std::cos(latitude);
        T b = radius * cl * std::cos(longitude);
        T c = radius * cl * std::sin(longitude);
        T d = radius * std::sin(latitude);

        return {t, b, c, d};
    }

    template<typename T>
    Quaternion<T> Cylindrical(T r, T angle, T h1, T h2) {

        T a = r * std::cos(angle);
        T b = r * std::sin(angle);

        return {a, b, h1, h2};
    }

    /**
     * The polar representation of a Quaternion.
     */
    template<typename T>
    using PolarRepresentation = std::array<T, 5>;

    /**
     * The polar representation of a Quaternion.
     * Returns 5 numbers:
     * - the Euclidean norm of the Quaternion,
     * - the polar angle theta,
     * - and each of the components of the "unreal unit direction".
     */
    template<typename T>
    PolarRepresentation<T> ToPolarRepresentation(const Quaternion<T>& x)
    {
        T nu = x.UnrealNormSquared();
        T n = std::sqrt(nu + x.a() * x.a());
        assert(nu >= 0);
        if (nu > 0)
        {
            T theta = std::acos(x.a() / n);
            T ns = sqrt(nu);
            return {{n, theta, x.b() / ns, x.c() / ns, x.d() / ns}};
        }
        const T pi = std::atan2(+0., -0.);

        return {{n, n == x.a() ? 0 : pi, 0, 0, 0}};
    }

    /**
     * Type used for 2x2 complex matrix representations of Quaternions.
     */
    template<typename T>
    using ComplexMatrix2D = std::array<std::array<std::complex<T>, 2>, 2>;

    /**
     * Returns a 2x2 complex matrix representation of a Quaternion x:
     * [ a + b i,  c + d i]
     * [ -c + d i, a - b i]
     */
    template<typename T>
    ComplexMatrix2D<T> ToComplexMatrix2D(const Quaternion<T>& x)
    {
        ComplexMatrix2D<T> cm;
        cm[0][0] = {x.a(), x.b()}; cm[0][1] = {x.c(), x.d()};
        cm[1][0] = -Conj(cm[0][1]); cm[1][1] = Conj(cm[0][0]);

        return cm;
    }

    /**
     * Returns a Quaternion from a 2x2 complex matrix cm:
     * [ a + b i,  c + d i]
     * [ -c + d i, a - b i]
     */
    template <typename T>
    Quaternion<T> FromComplexMatrix2D(const ComplexMatrix2D<T>& cm)
    {
        assert(cm[1][1] == Conj(cm[0][0]) && cm[1][0] == -Conj(cm[0][1]));
        return {cm[0][0].Real(), cm[0][0].Imag(), cm[0][1].Real(), cm[0][1].Imag()};
    }

    /**
     * Type used for 4x4 real matrix representation of a quaternion.
     */
    template <typename T>
    using RealMatrix4D = std::array<std::array<T,4>,4>;

    /**
     * Returns a 4x4 real matrix representation from a Quaternion x:
     * [ a  b  c  d ]
     * [-b  a -d  c ]
     * [-c  d  a -b ]
     * [-d -c  b  a ]
     */
    template <typename T>
    RealMatrix4D<T> ToRealMatrix4D(const Quaternion<T>& x)
    {
        RealMatrix4D<T> rm;
        rm[0] = {{x.a(), x.b(), x.c(), x.d()}};
        rm[1] = {{-x.b(), x.a(), -x.d(), x.c()}};
        rm[2] = {{-x.c(), x.d(), x.a(), -x.b()}};
        rm[3] = {{-x.d(), -x.c(), x.b(), x.a()}};

        return rm;
    }

    /**
     * Returns a Quaternion from a 4x4 real matrix rm:
     * [ a  b  c  d ]
     * [-b  a -d  c ]
     * [-c  d  a -b ]
     * [-d -c  b  a ]
     */
    template <typename T>
    Quaternion<T> FromRealMatrix4D(const RealMatrix4D<T>& rm)
    {
        return {rm[0][0],rm[0][1],rm[0][2],rm[0][3]};
    }

    /**
     * A 3D rotation matrix.
     */
    template<typename T>
    using RotationMatrix = std::array<std::array<T, 3>, 3>;

    /**
     * Returns a 3D rotation matrix.
     * This is the "homogeneous" expression to convert to a rotation matrix,
     * which works if the Quaternoin is not a unit Quaternion.
     */
    template<typename T>
    RotationMatrix<T> ToRotationMatrix(const Quaternion<T>& x)
    {
        T a2 = x.a() * x.a(), b2 = x.b() * x.b(), c2 = x.c() * x.c(), d2 = x.d() * x.d();
        T ab = x.a() * x.b(), ac = x.a() * x.c(), ad = x.a() * x.d();
        T bc = x.b() * x.c(), bd = x.b() * x.d();
        T cd = x.c() * x.d();
        std::array<T, 3> r0{{a2 + b2 - c2 - d2, 2 * (bc - ad), 2 * (bd + ac)}};
        std::array<T, 3> r1{{2 * (bc + ad), a2 - b2 + c2 - d2, 2 * (cd - ab)}};
        std::array<T, 3> r2{{2 * (bd - ac), 2 * (cd + ab), a2 - b2 - c2 + d2}};

        return {{r0, r1, r2}};
    }

    template<typename T>
    Quaternion<T> FromRotationMatrix(const RotationMatrix<T>& rm)
    {
        T t = rm[0][0] + rm[1][1] + rm[2][2];
        if (t > 0)
        {
            T s = (T)0.5 / std::sqrt(t + 1);
            return { (T)0.25 / s,
                     (rm[2][1] - rm[1][2]) * s,
                     (rm[0][2] - rm[2][0]) * s,
                     (rm[1][0] - rm[0][1]) * s};
        }

        if (rm[0][0] > rm[1][1] && rm[0][0] > rm[2][2])
        {
            T s = (T)2.0 * std::sqrt((T)1.0 + rm[0][0] - rm[1][1] - rm[2][2]);
            return {(rm[2][1] - rm[1][2]) / s,
                    (T)0.25 * s,
                    (rm[0][1] + rm[1][0]) / s,
                    (rm[0][2] + rm[2][0]) / s};
        }

        if (rm[1][1] > rm[2][2])
        {
            T s = (T)2.0 * std::sqrt((T)1.0 + rm[1][1] - rm[0][0] - rm[2][2]);
            return {(rm[0][2] - rm[2][0]) / s,
                    (rm[0][1] + rm[1][0]) / s,
                    (T)0.25 * s,
                    (rm[1][2] + rm[2][1]) / s};
        }
            T s = (T)2.0 * std::sqrt((T)1.0 + rm[2][2] - rm[0][0] - rm[1][1]);
            return {(rm[1][0] - rm[0][1]) / s,
                    (rm[0][2] + rm[2][0]) / s,
                    (rm[1][2] + rm[2][1]) / s,
                    (T)0.25 * s};

    }

    /**
     * Returns three Euler angles {yaw, pitch, roll} in radians.
     * x is required to be a unit quaternion.
     *
     * WARNING: conversion to/from Euler angles is not ready.
     */
    template <typename T>
    std::array<T, 3> ToEuler(const Quaternion<T>& x, T eps = 1e-12)
    {
        assert(x.is_unit(eps));
        auto pi = std::numbers::pi;

        T v = x.b()*x.c()+x.a()*x.d();
        if (std::abs(v - 0.5) < eps)
            return {{2*atan2(x.b(),x.a()), +pi/2, 0}};

        if (std::abs(v + 0.5) < eps)
            return {{-2*atan2(x.b(),x.a()), -pi/2, 0}};

        return {{atan2(2*(x.a()*x.c() - x.b()*x.d()), 1-2*(x.c()*x.c()+x.d()*x.d())),
                 std::asin(2*v),
                 atan2(2*(x.a()*x.b()-x.c()*x.d()), 1-2*(x.b()*x.b()+x.d()*x.d()))}};
    }

    /**
     * Returns a unit quaternion corresponding to the three Euler angles
     * {yaw, pitch, roll} expressed in radians.
     * The conventions used are with the 3,2,1 convention ??? TODO: verify
     */
    template <typename T>
    Quaternion<T> FromEuler(const std::array<T, 3>& x)
    {
        T c0 = std::cos(x[0]/2), s0 = std::sin(x[0]/2);
        T c1 = std::cos(x[1]/2), s1 = std::sin(x[1]/2);
        T c2 = std::cos(x[2]/2), s2 = std::sin(x[2]/2);
        T c0c1 = c0*c1, s0s1 = s0*s1, s0c1 = s0*c1, c0s1 = c0*s1;

        return {c0c1*c2+s0s1*s2,s0s1*c2-c0c1*s2,c0s1*c2+s0c1*s2,c0c1*s2-s0s1*c2};
    }

    /**
     * Hash of a quaternion - that makes it possible to use quaternions
     * as keys in std::set/std::map, if ever needed.
     *
     * fash-hash.
     */
    template <typename T>
    struct hash : std::function<Quaternion<T>(size_t)>
    {

        size_t operator()(const Quaternion<T>& x) const
        {

            auto mix = [](uint64_t h)
            {
                (h) ^= (h) >> 23;
                (h) *= 0x2127599bf4325c37ULL;
                (h) ^= (h) >> 47;
                return h;
            };

            const uint64_t len = 4 * sizeof(T); // in bytes, Qf is 4*4, Qd is 4*8, and Qld 4*16
            const uint64_t m = 0x880355f21e6d1965ULL;
            const uint64_t *pos = (const uint64_t *) &x;
            const uint64_t *end = pos + (len / 8);
            uint64_t h = 31 ^(len * m);
            uint64_t v;

            while (pos != end)
            {
                v = *pos++;
                h ^= mix(v);
                h *= m;
            }

            return mix(h);
        }
    };

    /**
     * Lexicographic order on quaternions, which is a total order, but not compatible
     * with the field structure.
     */
    template <typename T>
    struct LexicographicOrder : std::function<Quaternion<T>(Quaternion<T>, bool)>
    {
        constexpr bool operator()(const Quaternion<T>& x, const Quaternion<T>& y) const
        {
            return x.a() < y.a()
                   || (x.a() == y.a() && x.b() < y.b())
                   || (x.a() == y.a() && x.b() == y.b() && x.c() < y.c())
                   || (x.a() == y.a() && x.b() == y.b() && x.c() == y.c() && x.d() < y.d());
        }
    };

    /** +
     * Returns the conjugate of x, as a new Quaternion (x is unchanged).
     */
    template<typename T>
    Quaternion<T> Conj(const Quaternion<T>& x)
    {
        return {x.a(), -x.b(), -x.c(), -x.d()};
    }

    template<typename T>
    T NormSquared(const Quaternion<T>& x)
    {
        return x.NormSquared();
    }

    template<typename T>
    T Abs(const Quaternion<T>& x)
    {
        return x.Abs();
    }

    template<typename T>
    T UnrealNormSquared(const Quaternion<T>& x)
    {
        return x.UnrealNormSquared();
    }

    template<typename T>
    T NormL0(const Quaternion<T>& x)
    {
        return (x.a() != 0) + (x.b() != 0) + (x.c() != 0) + (x.d() != 0);
    }

    template<typename T>
    T NormL1(const Quaternion<T>& x)
    {
        return std::abs(x.a()) + std::abs(x.b()) + std::abs(x.c()) + std::abs(x.d());
    }

    template<typename T, typename T1>
    T NormLk(const Quaternion<T>& x, T1 k)
    {
        return std::pow(std::pow(std::abs(x.a()), k)
                        + std::pow(std::abs(x.b()), k)
                        + std::pow(std::abs(x.c()), k)
                        + std::pow(std::abs(x.d()), k), 1.0 / k);
    }

    template<typename T>
    T NormSup(const Quaternion<T>& x)
    {
        return std::max(std::max(std::abs(x.a()), std::abs(x.b())),
                        std::max(std::abs(x.c()), std::abs(x.d())));
    }

    template <typename T, typename T1 =T>
    bool IsZero(const Quaternion<T>& x, T1 eps = 0)
    {
        return x.IsZero(eps);
    }

    template <typename T, typename T1 =T>
    bool IsNonZero(const Quaternion<T>& x, T1 eps = 0)
    {
        return x.IsNonZero(eps);
    }

    template <typename T>
    bool IsNan(const Quaternion<T>& x)
    {
        return x.IsNan();
    }

    template <typename T>
    bool IsInf(const Quaternion<T>& x)
    {
        return x.IsInf();
    }

    template <typename T>
    bool IsFinite(const Quaternion<T>& x)
    {
        return x.IsFinite();
    }

    template<typename T, typename T1 =T>
    bool IsUnit(const Quaternion<T>& x, T1 eps = 0)
    {
        return x.IsUnit(eps);
    }

    template<typename T, typename T1 =T>
    bool IsReal(const Quaternion<T>& x, T1 eps = 0)
    {
        return x.IsReal(eps);
    }

    template<typename T, typename T1 =T>
    bool IsComplex(const Quaternion<T>& x, T1 eps = 0)
    {
        return x.IsComplex(eps);
    }

    template<typename T, typename T1 =T>
    bool IsUnreal(const Quaternion<T>& x, T1 eps = 0)
    {
        return x.IsUnreal(eps);
    }

    /**
     * Equality:
     * - Quaternion <-> real
     * - Quaternion <-> complex
     * - Quaternion <-> quaternion
     */

    /**
     * Quaternion <-> real
     *
     * For these equality operators, the goal is to allow e.g.
     * Qd(0,0,0,0) == 0, which is very nice to read, and pretty unambiguous,
     * but we must take care that the compiler doesn't try to use these with
     * T2 being an "exotic" type that we wouldn't want. That's why we have
     * "IS_CONVERTIBLE".
     *
     * operator== returns false if the Quaternion is not real.
     * If the Quaternion is real, it returns true if x.a() == y.
     */
    template <typename T, typename T2, IS_CONVERTIBLE(T2, T)>
    bool operator==(const Quaternion<T>& x, T2 y)
    {
        return x.is_real() && x.a() == y;
    }

    template <typename T, typename T2, IS_CONVERTIBLE(T2, T)>
    bool operator==(T2 y, const Quaternion<T>& x)
    {
        return x == y;
    }

    template <typename T, typename T2, IS_CONVERTIBLE(T2, T)>
    bool operator!=(const Quaternion<T>& x, T2 y)
    {
        return !(x == y);
    }

    template <typename T, typename T2, IS_CONVERTIBLE(T2, T)>
    bool operator!=(T2 y, const Quaternion<T>& x)
    {
        return !(x == y);
    }

    template <typename T, typename T2, typename T3, IS_CONVERTIBLE(T2, T), IS_CONVERTIBLE(T3,T)>
    bool NearlyEqual(const Quaternion<T>& x, T2 y, T3 eps)
    {
        return x.IsReal() && IsNearlyEqual(x.a(), y, eps);
    }

    template <typename T, typename T2, typename T3, IS_CONVERTIBLE(T2, T), IS_CONVERTIBLE(T3,T)>
    bool NearlyEqual(T2 y, const Quaternion<T>& x, T3 eps)
    {
        return x.IsReal() && IsNearlyEqual(x.a(), y, eps);
    }


    template<typename T, typename T2, IS_CONVERTIBLE(T2,T)>
    bool operator==(const Quaternion<T>& x, const std::complex<T2>& y)
    {
        return IsComplex(x) && x.a() == y.real() && x.b() == y.imag();
    }

    template<typename T, typename T2, IS_CONVERTIBLE(T2,T)>
    bool operator!=(const Quaternion<T>& x, const std::complex<T2>& y)
    {
        return !(x == y);
    }

    template<typename T, typename T2, IS_CONVERTIBLE(T2,T)>
    bool operator==(const std::complex<T2>& y, const Quaternion<T>& x)
    {
        return x == y;
    }

    template<typename T, typename T2, IS_CONVERTIBLE(T2,T)>
    bool operator!=(const std::complex<T2>& y, const Quaternion<T>& x)
    {
        return x != y;
    }

    template <typename T, typename T2, typename T3, IS_CONVERTIBLE(T2, T), IS_CONVERTIBLE(T3,T)>
    bool NearlyEqual(const Quaternion<T>& x, const std::complex<T2>& y, T3 eps)
    {
        return IsComplex(x, eps)
               && IsNearlyEqual(x.a(), y.real(), eps)
               && IsNearlyEqual(x.b(), y.imag(), eps);
    }

    template <typename T, typename T2, typename T3, IS_CONVERTIBLE(T2, T), IS_CONVERTIBLE(T3,T)>
    bool NearlyEqual(const std::complex<T2>& y, const Quaternion<T>& x, T3 eps)
    {
        return NearlyEqual(x, y, eps);
    }


    template<typename T1, typename T2>
    bool operator==(const Quaternion<T1>& x, const Quaternion<T2>& y)
    {
        return x.a() == y.a() && x.b() == y.b() && x.c() == y.c() && x.d() == y.d();
    }

    template<typename T>
    bool operator!=(const Quaternion<T>& x, const Quaternion<T>& y)
    {
        return !(x == y);
    }

    template <typename T1, typename T2, typename T3>
    bool NearlyEqual(const Quaternion<T1>& x, const Quaternion<T2>& y, T3 eps)
    {
        return IsNearlyEqual(x.a(), y.a(), eps)
               && IsNearlyEqual(x.b(), y.b(), eps)
               && IsNearlyEqual(x.c(), y.c(), eps)
               && IsNearlyEqual(x.d(), y.d(), eps);
    }

    template<typename T, typename T1>
    Quaternion<T> operator+(const Quaternion<T>& x, T1 y)
    {
        return Quaternion<T>(x) += y;
    }

    template<typename T, typename T1>
    Quaternion<T> operator+(T1 y, const Quaternion<T>& x)
    {
        return x + y;
    }

    template<typename T>
    Quaternion<T> operator+(const Quaternion<T>& x, std::complex<T>& y)
    {
        return Quaternion<T>(x) += y;
    }

    template<typename T>
    Quaternion<T> operator+(std::complex<T>& y, const Quaternion<T>& x)
    {
        return x + y;
    }

    template<typename T>
    Quaternion<T> operator+(const Quaternion<T>& x, const Quaternion<T>& y)
    {
        return Quaternion<T>(x) += y;
    }

    template<typename T, typename T1>
    Quaternion<T> operator-(const Quaternion<T>& x, T1 y)
    {
        return Quaternion<T>(x) -= y;
    }

    template<typename T, typename T1>
    Quaternion<T> operator-(T1 y, const Quaternion<T>& x)
    {
        return Quaternion<T>(x) += -y;
    }

    template<typename T>
    Quaternion<T> operator-(const Quaternion<T>& x, std::complex<T>& y)
    {
        return Quaternion<T>(x) -= y;
    }

    template<typename T>
    Quaternion<T> operator-(std::complex<T>& y, const Quaternion<T>& x)
    {
        return x + (-y);
    }

    template<typename T>
    Quaternion<T> operator-(const Quaternion<T>& x, const Quaternion<T>& y)
    {
        return Quaternion<T>(x) -= y;
    }

    template<typename T, typename T1>
    Quaternion<T> operator*(const Quaternion<T>& x, T1 y)
    {
        return Quaternion<T>(x) *= y;
    }

    template<typename T, typename T1>
    Quaternion<T> operator*(T1 y, const Quaternion<T>& x)
    {
        return x * y;
    }

    template<typename T>
    Quaternion<T> operator*(const Quaternion<T>& x, std::complex<T>& y)
    {
        return Quaternion<T>(x) *= y;
    }

    template<typename T>
    Quaternion<T> operator*(std::complex<T>& y, const Quaternion<T>& x)
    {
        return x * y;
    }

    template<typename T>
    Quaternion<T> operator*(const Quaternion<T>& x, const Quaternion<T>& y)
    {
        return Quaternion<T>(x) *= y;
    }

    template<typename T>
    Quaternion<T> Inverse(const Quaternion<T>& x)
    {
        return conj(x) / norm_squared(x);
    }

    template<typename T, typename T1>
    Quaternion<T> operator/(const Quaternion<T>& x, T1 y)
    {
        return Quaternion<T>(x) /= y;
    }

    template<typename T, typename T1>
    Quaternion<T> operator/(T1 y, const Quaternion<T>& x)
    {
        return y * inverse(x);
    }

    template<typename T>
    Quaternion<T> operator/(const Quaternion<T>& x, std::complex<T>& y)
    {
        return Quaternion<T>(x) /= y;
    }

    template<typename T>
    Quaternion<T> operator/(std::complex<T>& y, const Quaternion<T>& x)
    {
        return y * inverse(x);
    }

    template<typename T>
    Quaternion<T> operator/(const Quaternion<T>& x, const Quaternion<T>& y)
    {
        return x * inverse(y);
    }

    template<typename T>
    T Dot(const Quaternion<T>& x, const Quaternion<T>& y)
    {
        return x.a() * y.a() + x.b() * y.b() + x.c() * y.c() + x.d() * y.d();
    }

    /**
     * 9 operations
     */
    template<typename T>
    Quaternion<T> Cross(const Quaternion<T>& x, const Quaternion<T>& y)
    {
        return {0,
                x.c() * y.d() - x.d() * y.c(),
                x.d() * y.b() - x.b() * y.d(),
                x.b() * y.c() - x.c() * y.b()};
    }

    template<typename T>
    Quaternion<T> Commutator(const Quaternion<T>& x, const Quaternion<T>& y)
    {
        return x * y - y * x;
    }

    template<typename T>
    Quaternion<T> Normalize(const Quaternion<T>& x)
    {
        assert(Abs(x) > 0);
        return x / Abs(x);
    }


    template<typename T>
    Quaternion<T> Exp(const Quaternion<T>& x)
    {
        T un = x.UnrealNormSquared();
        if (un == 0)
            return {std::exp(x.a())};

        T n1 = std::sqrt(un);
        T ea = std::exp(x.a());
        T n2 = ea * std::sin(n1) / n1;
        return {ea * std::cos(n1), n2 * x.b(), n2 * x.c(), n2 * x.d()};
    }

    template<typename T>
    Quaternion<T> Log(const Quaternion<T>& x)
    {
        T nu2 = x.UnrealNormSquared();
        if (nu2 == 0)
        {
            if (x.a() > 0)
                return {std::log(x.a())};

            std::complex<T> l = log(std::complex<T>(x.a(), 0));
            return {l.real(), l.imag()};
        }

        T a = x.a();
        assert(nu2 > 0);
        T n = std::sqrt(a * a + nu2);
        T th = std::acos(a / n) / std::sqrt(nu2);
        return {std::log(n), th * x.b(), th * x.c(), th * x.d()};
    }

    /**
     * 10 operations:
     * a^2 - b^2 - c^2 - d^2
     * 2 a b
     * 2 a c
     * 2 a d
     */
    template<typename T>
    Quaternion<T> Pow2(const Quaternion<T>& x)
    {
        T aa = 2 * x.a();
        return {x.a() * x.a() - x.UnrealNormSquared(),
                aa * x.b(),
                aa * x.c(),
                aa * x.d()};
    }

    /**
     * 14 operations:
     * a (a^2 - 3 (b^2 + c^2 + d^2))
     * -b (-3 a^2 + b^2 + c^2 + d^2)
     * -c (-3 a^2 + b^2 + c^2 + d^2)
     * -d (-3 a^2 + b^2 + c^2 + d^2)
     */
    template<typename T>
    Quaternion<T> Pow3(const Quaternion<T>& x)
    {
        T a2 = x.a() * x.a();
        T n1 = x.UnrealNormSquared();
        T n2 = 3 * a2 - n1;
        return {x.a() * (a2 - 3 * n1),
                x.b() * n2,
                x.c() * n2,
                x.d() * n2};
    }

    /**
     * 18 operations:
     * a^4 - 6 a^2 (b^2 + c^2 + d^2) + (b^2 + c^2 + d^2)^2
     * -4 a b (-a^2 + b^2 + c^2 + d^2)
     * -4 a c (-a^2 + b^2 + c^2 + d^2)
     * -4 a d (-a^2 + b^2 + c^2 + d^2)
     */
    template<typename T>
    Quaternion<T> Pow4(const Quaternion<T>& x)
    {
        T a2 = x.a() * x.a();
        T n1 = x.UnrealNormSquared();
        T n2 = 4 * x.a() * (a2 - n1);
        return {a2 * a2 - 6 * a2 * n1 + n1 * n1,
                x.b() * n2,
                x.c() * n2,
                x.d() * n2};
    }


    template<typename T>
    Quaternion<T> Pow(const Quaternion<T>& x, int expt)
    {
        if (expt < 0)
            return Inverse(pow(x, -expt));
        if (expt == 0)
            return {1};
        if (expt == 1)
            return x;
        if (expt == 2)
            return Pow2(x);
        if (expt == 3)
            return Pow3(x);
        if (expt == 4)
            return Pow4(x);

        Quaternion<T> x4 = Pow4(x), y = x4;

        for (int i = 1; i < expt / 4; ++i)
            y *= x4;
        if (expt % 4 == 3)
            y *= Pow3(x);
        if (expt % 4 == 2)
            y *= Pow2(x);
        if (expt % 4 == 1)
            y *= x;
        return y;
    }

    template <typename T, typename T1, std::enable_if<!std::is_same_v<T1, int>()>* = nullptr>
    Quaternion<std::common_type_t<T,T1>> Pow(const Quaternion<T>& x, T1 a)
    {
        Quaternion<std::common_type_t<T,T1>> xx(x);
        if (std::floor(a) == a)
            return pow(x, (int) a);

        return exp(a * log(xx));
    }


    template<typename T>
    Quaternion<T> Pow(const Quaternion<T>& x, const Quaternion<T>& a)
    {
        if (a.IsReal())
            return Pow(x, a.a());

        return Exp(a * Log(x));
    }

    template<typename T>
    Quaternion<T> Cos(const Quaternion<T>& x)
    {
        T z = x.UnrealNormSquared();
        if (z == 0)
            return {std::cos(x.a())};

        z = std::sqrt(z);
        T w = -std::sin(x.real()) * std::sinh(z) / z;
        return {std::cos(x.real()) * std::cosh(z), w * x.b(), w * x.c(), w * x.d()};
    }

    template<typename T>
    Quaternion<T> Sin(const Quaternion<T>& x)
    {
        T z = x.UnrealNormSquared();
        if (z == 0)
            return {std::sin(x.a())};

        z = std::sqrt(z);
        T w = std::cos(x.real()) * std::sinh(z) / z;
        return {std::sin(x.real()) * std::cosh(z), w * x.b(), w * x.c(), w * x.d()};
    }

    template<typename T>
    Quaternion<T> Tan(const Quaternion<T>& x)
    {
        T z = x.UnrealNormSquared();
        if (z == 0)
            return {std::tan(x.a())};

        z = std::sqrt(z);
        T n = std::sinh(2 * z);
        T d = std::cos(2 * x.a()) + std::cosh(2 * z);
        T r = n / (z * d);
        return {std::sin(2 * x.a()) / d, r * x.b(), r * x.c(), r * x.d()};
    }

    template<typename T>
    Quaternion<T> Cosh(const Quaternion<T>& x)
    {
        return (Exp(x) + Exp(-x)) / 2;
    }

    template<typename T>
    Quaternion<T> Sinh(const Quaternion<T>& x)
    {
        return (Exp(x) - Exp(-x)) / 2;
    }

    template<typename T>
    Quaternion<T> Tanh(const Quaternion<T>& x)
    {
        return Sinh(x) / Cosh(x);
    }

    template<typename T, typename K>
    Quaternion<T> AXBY(K k1, const Quaternion<T>& x, K k2, const Quaternion<T>& y)
    {

        T a = k1 * x.a() + k2 * y.a();
        T b = k1 * x.b() + k2 * y.b();
        T c = k1 * x.c() + k2 * y.c();
        T d = k1 * x.d() + k2 * y.d();

        return {a, b, c, d};
    }


    struct QuaternionIO
    {
        static long double scalar_zero_threshold;
        static int print_style;


        template<typename T>
        static std::ostream& Print(std::ostream& out, const Quaternion <T>& q)
        {
            if (print_style == 0)
            {
                if (q == 0)
                    return out << 0;
                if (q == Quaternion<T>(1))
                    return out << 1;
                if (q == Quaternion<T>(-1))
                    return out << -1;
                if (q == Quaternion<T>(0, 1))
                    return out << "i";
                if (q == Quaternion<T>(0, -1))
                    return out << "-i";
                if (q == Quaternion<T>(0, 0, 1))
                    return out << "j";
                if (q == Quaternion<T>(0, 0, -1))
                    return out << "-j";
                if (q == Quaternion<T>(0, 0, 0, 1))
                    return out << "k";
                if (q == Quaternion<T>(0, 0, 0, -1))
                    return out << "-k";
                auto s = [](T x) { return x < 0 ? "" : "+"; };
                if (!IsScalarZero(q.a(), QuaternionIO::scalar_zero_threshold))
                    out << q.a();
                if (!IsScalarZero(q.b(), QuaternionIO::scalar_zero_threshold))
                    out << s(q.b()) << q.b() << "i";
                if (!IsScalarZero(q.c(), QuaternionIO::scalar_zero_threshold))
                    out << s(q.c()) << q.c() << "j";
                if (!IsScalarZero(q.d(), QuaternionIO::scalar_zero_threshold))
                    out << s(q.d()) << q.d() << "k";
            } else if (print_style == 1) {
                out << "{" << q.a() << "," << q.b() << "," << q.c() << "," << q.d() << "}";
            }
            return out;
        }
    };

    /**
     * IO manipulators to control the format when printing quaternions out to a stream.
     */
    long double QuaternionIO::scalar_zero_threshold = 0;
    int QuaternionIO::print_style;

    struct SetScalarZeroThreshold
    {
        long double eps = 0;
    };

    inline SetScalarZeroThreshold SetDisplayEps(long double eps)
    {
        SetScalarZeroThreshold sszt;
        sszt.eps = eps;
        return sszt;
    }

    inline std::ostream& operator<<(std::ostream& out, SetScalarZeroThreshold sszt)
    {
        QuaternionIO::scalar_zero_threshold = sszt.eps;
        return out;
    }

    enum class DisplayStyle { q_nice, q_compact};

    struct SetDisplayStyle
    {
        DisplayStyle style;
    };

    inline SetDisplayStyle SetStyle(DisplayStyle ds)
    {
        SetDisplayStyle sds;
        sds.style = ds;
        return sds;
    }

    inline std::ostream& operator<<(std::ostream& out, SetDisplayStyle sds)
    {
        QuaternionIO::print_style = sds.style;
        return out;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const Quaternion <T>& q)
    {
        return QuaternionIO::print(out, q);
    }
}