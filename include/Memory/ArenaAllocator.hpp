/*
 * Project: TestProject
 * File: ArenaAllocator.hpp
 * Author: olegfresi
 * Created: 24/10/25 14:27
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
#include <tuple>
#include <type_traits>
#include <memory>
#include <new>
#include <array>
#include <cstddef>

#include "../Application/Pointers.hpp"

/*  Idea of Arena Allocator
 *
 *--------------------------------------------------------------------------------*/
namespace lux
{
    class Arena
    {
    public:
        explicit Arena(size_t size) : buffer{static_cast<std::byte*>(operator new(size))}, capacity{size} {}

        ~Arena()
        {
            CallDestructors();
            operator delete(buffer);
        }

        template<typename T, typename... Args>
        T* Create(Args&&... args)
        {
            static_assert(std::is_constructible_v<T, Args...>, "T must be constructible from Args");

            auto [ptr, new_offset] = Allocate(offset, sizeof(T), alignof(T));

            if constexpr (std::is_trivially_destructible_v<T>)
            {
                T* obj = new (ptr) T(std::forward<Args>(args)...);
                offset = new_offset;
                return obj;
            }
            else
            {
                auto [d_node_ptr, final_offset] = Allocate(offset, sizeof(DestructorNode), alignof(DestructorNode));
                T* obj = new (ptr) T(std::forward<Args>(args)...);

                auto dtor_call = [](void* p) { static_cast<T*>(p)->~T(); };
                DestructorNode* new_node  = new (d_node_ptr) DestructorNode{dtor_call, tail, obj};
                tail = new_node;
                offset = final_offset;
                return obj;
            }
        }

        void Reset()
        {
            offset = 0;
            CallDestructors();
        }

        Arena(const Arena&) = delete;
        Arena& operator=(const Arena&) = delete;

        Arena(Arena&&) = delete;
        Arena& operator=(Arena&&) = delete;

    private:

        struct DestructorNode
        {
            void (*dtor)(void*);
            DestructorNode* prev;
            void* obj;
        };

        std::byte* buffer;
        size_t capacity;
        size_t offset = 0;
        DestructorNode* tail = nullptr;

        void CallDestructors()
        {
            while(tail)
            {
                tail->dtor(tail->obj);
                tail = tail->prev;
            }
        }

        std::pair<void*, size_t> Allocate(size_t currOffset, size_t size, size_t alignment) const
        {
            std::byte* currentPtr = buffer + currOffset;
            size_t space = capacity - currOffset;
            void* alignedPtr = currentPtr;

            if (std::align(alignment, size, alignedPtr, space) == nullptr)
                throw std::bad_alloc();

            auto new_offset = static_cast<std::byte*>(alignedPtr) - buffer + size;

            return {alignedPtr, new_offset};
        }
    };

    using word_t = uintptr_t;

    struct Block
    {
        bool used;
        size_t size;
        size_t alignment;
        NonOwnPtr<Block> next = nullptr;

        std::array<word_t, 1> data;
    };
}