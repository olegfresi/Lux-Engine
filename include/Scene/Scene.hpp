/*
 * Project: TestProject
 * File: Scene.hpp
 * Author: olegfresi
 * Created: 08/04/25 17:51
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
#include "../Renderer/Light/Light.hpp"
#include "../Renderer/Camera/Camera.hpp"
#include "../Renderer/Mesh/Mesh.hpp"
#include "../Renderer/Primitives/IPrimitive.hpp"
#include "SkyBox.hpp"

namespace lux
{
    struct NewMeshInstance;
    class GameObject;

    struct SceneObject
    {
        NonOwnPtr<Mesh> mesh;
        Transform transform;
    };

    class Scene
    {
    public:
        explicit Scene(const std::string& name);
        Scene(const std::string& name, NonOwnPtr<Camera> camera);

        static void LoadScene() noexcept {}
        static void UnloadScene() noexcept {}
        static void Terminate() noexcept {}
        static void Update() noexcept {}

        static void SaveToFile(const std::string& filePath) noexcept {}
        static void LoadFromFile(const std::string& filePath) noexcept {}

        static void AddGameObject(const Ref<GameObject>& gameObject) noexcept {}
        static void RemoveGameObject(const Ref<GameObject>& gameObject) noexcept {}
        void SetCamera(NonOwnPtr<Camera> camera) noexcept;

        void AddMesh(const Ref<Mesh>& mesh) noexcept;
        void RemoveMesh(const Ref<Mesh>& mesh) noexcept;
        void SetupMeshes() const noexcept;

        void AddPrimitive(const Ref<IPrimitive>& primitive) noexcept;
        void RemovePrimitive(const Ref<IPrimitive>& primitive) noexcept;

        static void AddLight(const Light& light) noexcept {}
        static void RemoveLight(const Light& light) noexcept {}

        static std::unordered_map<NonOwnPtr<Mesh>, std::vector<Transform>, MeshPtrHash, MeshPtrEq>
            GroupMeshInstances(const std::vector<SceneObject>& objects);

        const std::vector<Ref<Mesh>>& GetMeshes() const noexcept { return m_meshes; }
        const std::vector<Ref<IPrimitive>>& GetPrimitives() const noexcept { return m_primitives; }
        const std::vector<Light>& GetLights() const noexcept { return m_lights; }

        const Camera& GetCamera() const noexcept { return *m_camera; }

    private:
        std::string m_name;
        NonOwnPtr<Camera> m_camera;

        std::vector<Ref<Mesh>> m_meshes;
        std::vector<Ref<IPrimitive>> m_primitives;
        std::vector<Light> m_lights;
    };
}