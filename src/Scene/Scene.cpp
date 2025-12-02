#include "../../include/Scene/Scene.hpp"
#include "../../include/Renderer/Mesh/Mesh.hpp"

namespace lux
{
    Scene::Scene(const std::string &name) : m_name { name }, m_camera { nullptr }
    {

    }

    Scene::Scene(const std::string& name, NonOwnPtr<Camera> camera) : m_name { name }, m_camera { camera }
    {
        CORE_ASSERT(m_camera != nullptr, "Camera is null")
    }

    void Scene::AddMesh(const Ref<Mesh>& mesh) noexcept
    {
        m_meshes.push_back(mesh);
    }

    void Scene::RemoveMesh(const Ref<Mesh>& mesh) noexcept
    {
        auto new_end = std::ranges::remove_if(m_meshes, [&](const Ref<Mesh>& m) { return m == mesh; }).begin();
        m_meshes.erase(new_end, m_meshes.end());
    }

    void Scene::AddPrimitive(const Ref<IPrimitive>& primitive) noexcept
    {
        m_primitives.push_back(primitive);
    }

    void Scene::RemovePrimitive(const Ref<IPrimitive>& primitive) noexcept
    {
        std::input_or_output_iterator auto new_end = std::ranges::remove(m_primitives, primitive).begin();
        m_primitives.erase(new_end, m_primitives.end());
    }

    void Scene::SetupMeshes() const noexcept
    {
        CORE_ASSERT(m_camera != nullptr, "Camera is null")

        for (const auto& mesh : m_meshes)
        {
            mesh->GetShader()->Bind();
            mesh->GetShader()->SetUniform("model", math::Identity4f);
            mesh->GetShader()->SetUniform("view", m_camera->GetView());
            mesh->GetShader()->SetUniform("projection", m_camera->GetProjection());
            mesh->SetupMesh();
        }
    }

    std::unordered_map<NonOwnPtr<Mesh>, std::vector<Transform>, MeshPtrHash, MeshPtrEq>
    Scene::GroupMeshInstances(const std::vector<SceneObject>& objects)
    {
        std::unordered_map<NonOwnPtr<Mesh>, std::vector<Transform>, MeshPtrHash, MeshPtrEq> grouped;

        for (const auto& obj : objects)
        {
            auto it = grouped.find(obj.mesh);

            if (it != grouped.end())
                it->second.push_back(obj.transform);
            else
                grouped.emplace(obj.mesh, std::vector{ obj.transform });
        }

        return grouped;
    }

    void Scene::SetCamera(NonOwnPtr<Camera> camera) noexcept
    {
        CORE_ASSERT(camera != nullptr, "Camera is null")
        m_camera = camera;
    }
}