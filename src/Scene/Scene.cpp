#include "../../include/Scene/Scene.hpp"

namespace lux
{
    Scene::Scene(const std::string &name) : m_name { name }, m_camera { nullptr }
    {

    }

    Scene::Scene(const std::string& name, NonOwnPtr<Camera> camera) : m_name { name }, m_camera { camera }
    {
        CORE_ASSERT(m_camera != nullptr, "Camera is null")
    }

    void Scene::LoadScene() noexcept
    {

    }

    void Scene::UnloadScene() noexcept
    {

    }

    void Scene::Terminate() noexcept
    {

    }

    void Scene::Update() noexcept
    {

    }

    void Scene::SaveToFile(const std::string &filePath) noexcept
    {

    }

    void Scene::LoadFromFile(const std::string &filePath) noexcept
    {

    }

    void Scene::AddGameObject(const Ref<GameObject> &gameObject) noexcept
    {

    }

    void Scene::RemoveGameObject(const Ref<GameObject> &gameObject) noexcept
    {

    }

    void Scene::AddLight(const Light &light) noexcept
    {

    }

    void Scene::RemoveLight(const Light &light) noexcept
    {

    }

    void Scene::AddMesh(Ref<Mesh> mesh) noexcept
    {
        m_meshes.push_back(mesh);
    }

    void Scene::RemoveMesh(Ref<Mesh> mesh) noexcept
    {
        std::input_or_output_iterator auto new_end = std::ranges::remove(m_meshes, mesh).begin();
        m_meshes.erase(new_end, m_meshes.end());
    }

    void Scene::AddPrimitive(Ref<IPrimitive> primitive) noexcept
    {
        m_primitives.push_back(primitive);
    }

    void Scene::RemovePrimitive(Ref<IPrimitive> primitive) noexcept
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

    void Scene::SetCamera(NonOwnPtr<Camera> camera) noexcept
    {
        CORE_ASSERT(camera != nullptr, "Camera is null")
        m_camera = camera;
    }
}