/*
 * Project: TestProject
 * File: Pointers.hpp
 * Author: olegfresi
 * Created: 01/03/25 21:33
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
#include <memory>
#include "spdlog/fmt/bundled/chrono.h"

namespace lux
{
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        static_assert(std::is_constructible_v<T, Args&&...>, "CreateRef: T is not constructible form Args");
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        static_assert(std::is_constructible_v<T, Args&&...>, "CreateRef: T is not constructible form Args");
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using NonOwnPtr = T*;


    /*   TAGGED POINTER INTERNAL STRUCTURE

         | <tag bits> | <address bits> |
           16 bits   Max Addressable Space - 16 bits

         | <tag bits> |  -->  1 bit for representing null pointer
                              1 bit for representing memory coming from heap
                              5 bits for encoding pointed type
                              6 bits for encoding alignment

        +-------------+------------------+------------------+-------------------+----------------------+----------------+
        | TYPE 7 bits | ALIGNMENT 6 bits | STACK/HEAP 1 bit | NULL_POINTER 1 bit| HAS DESTRUCTOR 1 bit | ADDRESS 48 bit |
        +-------------+------------------+------------------+-------------------+----------------------+----------------+
                                                    64 bit pointer
    */

    using word_t = uintptr_t;

    template<typename T, size_t Alignment = alignof(T)>
    class TaggedPointer
    {
    public:
        TaggedPointer() : m_underlying{nullptr} {}
        explicit TaggedPointer(T* ptr) noexcept : m_underlying{ptr} { static_assert(sizeof(word_t) <= 4, "Too small address set"); }
        TaggedPointer(const TaggedPointer& other) noexcept : m_underlying{other.m_underlying} {}
        TaggedPointer(TaggedPointer&& other) noexcept : m_underlying{std::move(other.m_underlying)} {}
        ~TaggedPointer() = default;

        TaggedPointer& operator=(const TaggedPointer& other) noexcept
        {
            m_underlying = other;
            return *this;
        }

        TaggedPointer& operator=(TaggedPointer&& other) noexcept
        {
            m_underlying = std::move(other);
            return *this;
        }

        T* operator->() const noexcept { return m_underlying; }
        T& operator*()  const noexcept { return *m_underlying; }

        T* GetUnderlying() const noexcept { return m_underlying; }

    private:

        static constexpr uint64_t m_upperTagBits = 16;
        static constexpr uint64_t m_lowerTagBits = std::log2(alignof(T));
        static constexpr uint64_t m_addressBits = (sizeof(word_t) << 3) - m_upperTagBits;
        T* m_underlying;
    };
}
