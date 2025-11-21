/*
 * Project: TestProject
 * File: Application.hpp
 * Author: olegfresi
 * Created: 11/02/25 21:46
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
#include "Window.hpp"
#include "EngineSettings.hpp"
#include "Pointers.hpp"
#include "HashedString.hpp"
#include "Layer.hpp"
#include "../Event/EventSystem.hpp"
#include "../Input/InputDevice.hpp"
#include "Assertion.hpp"
#include "../Renderer/Camera/CameraController.hpp"
#include "../Input/Keyboard.hpp"
#include "../Input/Mouse.hpp"
#include "../Input/Gamepad.hpp"

namespace lux
{
    void Setup();

    class AppInterface;

    class AppContext
    {
    public:
        ~AppContext();

        std::vector<NonOwnPtr<AppInterface>> m_layers;
        EventDispatcher m_dispatcher;
    };

    class AppInterface
    {
    public:
        AppInterface() : m_context{new AppContext{}}, m_layerID{0} {}

        virtual ~AppInterface() = default;

        template <typename Layer>
        NonOwnPtr<Layer> GetLayer()
        {
            CORE_ASSERT((!std::is_base_of_v<AppInterface, Layer>), "Layer must derive from AppInterface")
            auto itr = std::find_if(m_context->m_layers.begin(), m_context->m_layers.end(), [this](auto layer)
            {
                return (layer->m_layerID == TypeID<Layer>());
            });

            if(itr != m_context->m_layers.end())
                return static_cast<Layer*>(*itr);

            return nullptr;
        }

        template <typename Layer, typename... Args >
        NonOwnPtr<Layer> AttachLayer(Args... args)
        {
            CORE_ASSERT(!(std::is_base_of_v<AppInterface, Layer>), "Layer must derive from AppInterface")
            if(GetLayer<Layer>() != nullptr)
            {
                CORE_ERROR("Layer Already Attached");
                return nullptr;
            }

            static_assert(std::is_constructible_v<Layer, Args...>, "Layer is not constructible with the provided arguments");
            auto layer = new Layer(std::forward<Args>(args)...);
            m_context->m_layers.push_back(layer);
            layer->m_layerID = TypeID<Layer>();
            layer->m_context = m_context;
            layer->OnStart();
            return layer;
        }

        template <typename Layer>
        void DetachLayer()
        {
            CORE_ASSERT((std::is_base_of_v<AppInterface, Layer>), "Layer must derive from AppInterface")
            m_context->m_dispatcher.PostTask([this]()
            {
                m_context->m_layers.erase(std::remove_if(m_context->m_layers.begin(), m_context->m_layers.end(), [this](const auto& layer)
                {
                    if(layer->m_id == TypeID<Layer>())
                    {
                        m_context->m_dispatcher.EraseListener( layer->m_id );

                        if(layer != nullptr)
                        {
                            delete layer;
                            layer = nullptr;
                            return true;
                        }
                        return false;
                    }
                }), m_context->m_layers.end());
            });
        }

    protected:
        void OnUpdate() {}
        void OnStart() {}

    private:
        friend class Application;
        NonOwnPtr<AppContext> m_context{};
        uint32_t m_layerID;
    };


    class Application : public AppInterface
    {
    public:
        Application();
        ~Application() override;

        void RunContext() const noexcept;
        void Run();
        void InputDeviceSetup();
        void InitSubSystems();

    private:
        WindowSettings settings {800, 600, "TestProject", false};

        NonOwnPtr<EventDispatcher> m_dispatcher;
        ActionMapper m_mapper;
        Scope<Window> m_window;
        Scope<InputDevice> m_inputDeviceSystem;

        Camera m_camera;
        CameraController m_cameraController;

        Scope<Mouse> m_mouse;
        Scope<Keyboard> m_keyboard;
        Scope<Gamepad> m_gamepad;

        bool isRunning = true;
        double m_lastFrameTime = 0.0f;
    };
}