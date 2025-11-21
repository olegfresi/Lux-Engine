/*
 * Project: TestProject
 * File: EventSystem.hpp
 * Author: olegfresi
 * Created: 02/03/25 17:54
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
#include <functional>
#include <queue>
#include <any>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include "../Application/HashedString.hpp"

namespace lux
{
    template<typename Event, typename... Args>
    using Callback = std::function<void(Event, Args... args)>;


    class EventDispatcher
    {
    private:
        struct ListenerWrapper
        {
            uint32_t id;
            std::function<void(std::any)> callback;
        };

        std::unordered_map<std::type_index , std::vector<ListenerWrapper>> mListeners;
        std::queue<std::any> mEventQueue;
        std::queue<std::function<void()>> mTasks;

    public:

        static uint32_t GenerateListenerID()
        {
            static std::atomic<uint32_t> idCounter { 0 };
            return idCounter++;
        }

        template <typename Event>
        void RegisterListener(uint32_t listenerId, std::function<void(const Event&)> callback)
        {
            auto wrapper = [callback](std::any event)
            {
                callback(std::any_cast<const Event&>(event));
            };
            mListeners[typeid(Event)].push_back({listenerId, wrapper});
        }


        template <typename Event>
        void UnregisterListener(uint32_t listenerId)
        {
            auto it = mListeners.find(typeid(Event));
            if (it != mListeners.end())
            {
                it->second.erase(std::remove_if(it->second.begin(), it->second.end(), [listenerId](const ListenerWrapper& listener)
                {
                    return listener.id == listenerId;
                }), it->second.end());
            }
        }

        void EraseListener(uint32_t listenerId)
        {
            for (auto& [type, listeners] : mListeners)
            {
                listeners.erase(std::remove_if(listeners.begin(), listeners.end(), [listenerId](const ListenerWrapper& listener)
                {
                    return listener.id == listenerId;
                }), listeners.end());
            }
        }

        template <typename Event, typename... Args>
        void PostEvent(Args&&... args)
        {
            static_assert(std::is_constructible_v<Event, Args&&...>, "PostEvent: Event is not constructible form Args");
            mEventQueue.push(Event(((std::forward<Args>( args )))...));
        }

        template <typename Task>
        void PostTask(Task&& task)
        {
            mTasks.push(std::forward<Task>(task));
        }

        void PollEvents()
        {
            while (!mEventQueue.empty())
            {
                std::any event = std::move(mEventQueue.front());
                mEventQueue.pop();

                auto it = mListeners.find(event.type());
                if (it != mListeners.end())
                {
                    for (const auto& listener : it->second)
                        listener.callback(event);
                }
            }
            return;
        }

        EventDispatcher() = default;
        ~EventDispatcher() = default;
        EventDispatcher(const EventDispatcher&) = delete;
        EventDispatcher& operator=(const EventDispatcher&) = delete;
        EventDispatcher(EventDispatcher&&) = default;
        EventDispatcher& operator=(EventDispatcher&&) = default;
    };
}