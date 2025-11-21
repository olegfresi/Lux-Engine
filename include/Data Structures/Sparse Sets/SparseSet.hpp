/*
 * Project: TestProject
 * File: SparseSet.hpp
 * Author: olegfresi
 * Created: 30/10/25 09:08
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
#include <vector>
#include <iterator>
#include <limits>
#include <algorithm>

namespace lux::bits
{
    template <typename T>
    static constexpr int LeastSigBit(T x)
    {
        static_assert(std::is_integral_v<T>, "LeastSigBit must be integral type");

        if (x == 0)
            return -1;

        return std::countr_zero(static_cast<std::make_unsigned_t<T>>(x));
    }

    template <typename T>
    static constexpr unsigned CountBits(T x)
    {
        return  std::popcount(x);
    }
}


class SparseSet
{
    static constexpr std::size_t EmptyIndex = static_cast<std::size_t>(-1);
    typedef std::size_t base_type;
    static constexpr unsigned unsigned_bits = std::numeric_limits<base_type>::digits;
    static constexpr unsigned unsigned_bits_log2 = (unsigned_bits == 64 ? 6 : unsigned_bits == 32 ? 5 : unsigned_bits == 16 ? 4 : 3);
    static constexpr unsigned unsigned_bits_log2_mask = 0xFFFFFFFF >> (32 - unsigned_bits_log2);
    static constexpr std::size_t one_bit = 1;

    unsigned m_size;
    std::vector<base_type> m_bit_array;
    mutable std::vector<std::size_t> m_sequence;
    mutable bool m_iterator_present;

    void CreateIterationSequence() const
    {
        if (m_sequence.capacity() != m_size)
            m_sequence.reserve(m_size);

        std::size_t k = 0;
        for (auto x : m_bit_array)
        {
            if (x)
            {
                unsigned bit = k;
                do
                {
                    unsigned p = lux::bits::LeastSigBit(x);
                    bit += p;
                    x >>= p;
                    m_sequence.push_back(bit);
                    if (x == 1)
                        break;
                    x >>= 1;
                    bit++;
                } while (true);
            }
            k += unsigned_bits;
        }
    }

public:
    typedef std::size_t value_type;
    typedef std::size_t key_type;
    typedef std::size_t size_type;
    typedef std::vector<std::size_t>::const_iterator iterator;
    typedef iterator const_iterator;

    typedef std::vector<std::size_t>::const_reverse_iterator reverse_iterator;
    typedef reverse_iterator const_reverse_iterator;

    SparseSet(std::size_t size) : m_size(size),
        m_bit_array((m_size + unsigned_bits - 1) / unsigned_bits), m_sequence(), m_iterator_present(false) {}

    SparseSet() : m_size(0), m_bit_array(0), m_sequence(), m_iterator_present(false) {}

    void Swap(SparseSet& s)
    {
        m_bit_array.swap(s.m_bit_array);
        m_sequence.swap(s.m_sequence);
        std::swap(m_iterator_present, s.m_iterator_present);
        std::swap(m_size, s.m_size);
    }

    void Resize(std::size_t size)
    {
        m_size = size;
        m_bit_array.resize((m_size + unsigned_bits - 1) / unsigned_bits);

        if (m_iterator_present)
        {
            m_iterator_present = false;
            m_sequence.clear();
        }
    }

    bool Insert(value_type i)
    {
        if (m_iterator_present)
        {
            m_iterator_present = false;
            m_sequence.clear();
        }

        base_type& v = m_bit_array[i >> unsigned_bits_log2];
        base_type x = v;
        v |= (one_bit << (i & unsigned_bits_log2_mask));
        return x != v;
    }

    void Erase(value_type i)
    {
        m_bit_array[i >> unsigned_bits_log2] &= ~(one_bit << (i & unsigned_bits_log2_mask));
        if (m_iterator_present)
        {
            m_iterator_present = false;
            m_sequence.clear();
        }
    }

    bool Test(std::size_t i) const
    {
        return ((m_bit_array[i >> unsigned_bits_log2] >> (i & unsigned_bits_log2_mask)) & 1) != 0;
    }

    bool Empty() const
    {
        for (auto x : m_bit_array)
            if (x != 0)
                return false;

        return true;
    }

    void Clear()
    {
        if (m_iterator_present)
        {
            m_iterator_present = false;
            m_sequence.clear();
        }

        std::ranges::fill(m_bit_array.begin(), m_bit_array.end(), 0);
    }

    std::size_t Size() const
    {
        return m_size;
    }

    std::size_t Count() const
    {
        if (m_iterator_present)
            return m_sequence.size();

        std::size_t count = 0;
        for (auto x : m_bit_array)
            count += lux::bits::CountBits(x);

        return count;
    }

    iterator Begin() const
    {
        if (!m_iterator_present)
        {
            CreateIterationSequence();
            m_iterator_present = true;
        }

        return m_sequence.begin();
    }

    iterator End() const
    {
        if (!m_iterator_present)
        {
            CreateIterationSequence();
            m_iterator_present = true;
        }

        return m_sequence.end();
    }

    reverse_iterator Rbegin() const
    {
        if (!m_iterator_present)
        {
            CreateIterationSequence();
            m_iterator_present = true;
        }

        return m_sequence.rbegin();
    }

    reverse_iterator Rend() const
    {
        if (!m_iterator_present)
        {
            CreateIterationSequence();
            m_iterator_present = true;
        }

        return m_sequence.rend();
    }

    iterator LowerBound(value_type i) const
    {
        return std::ranges::lower_bound(m_sequence.begin(), m_sequence.end(), i);
    }

    iterator UpperBound(value_type i) const
    {
        return std::ranges::upper_bound(m_sequence.begin(), m_sequence.end(), i);
    }
};

///
/// The unordered sparse set is slower than sparse set, except for iteration over the whole set of values.
/// It uses more memory than sparse set.
///
class UnorderedSparseSet
{
public:

    typedef std::vector<std::size_t*> direct_access_sequence;
    typedef std::vector<std::size_t>  iteration_sequence;

    direct_access_sequence m_sparse;
    iteration_sequence m_dense;

public:
    typedef std::size_t value_type;
    typedef std::size_t size_type;
    typedef std::size_t key_type;
    typedef iteration_sequence::const_iterator iterator;
    typedef iterator const_iterator;

    typedef iteration_sequence::const_reverse_iterator reverse_iterator;
    typedef reverse_iterator const_reverse_iterator;

    UnorderedSparseSet(std::size_t size) :m_sparse(size), m_dense()
    {
        m_dense.reserve(size);
    }

    UnorderedSparseSet() :m_sparse(), m_dense() {}

    void Resize(std::size_t size)
    {
        m_sparse.resize(size);
        m_dense.reserve(size);
        m_dense.clear();
    }

    void Swap(UnorderedSparseSet& s)
    {
        m_sparse.swap(s.m_sparse);
        m_dense.swap(s.m_dense);
    }

    bool Test(std::size_t i) const
    {
        return m_sparse[i] != nullptr;
    }

    bool Insert(std::size_t i)
    {
        if (m_sparse[i] != nullptr)
            return false;

        m_dense.push_back(i);
        m_sparse[i] = &m_dense.back();
        return true;
    }

    void Erase(std::size_t i)
    {
        if (m_sparse[i] == nullptr)
            return;

        std::size_t* v = m_sparse[i];
        std::size_t p_index = m_dense.back();
        if (*v != p_index)
        {
            *v = p_index;
            m_sparse[p_index] = v;
        }

        m_sparse[i] = nullptr;
        m_dense.erase(m_dense.end() - 1);
    }

    void Clear()
    {
        std::ranges::fill(m_sparse.begin(), m_sparse.end(), nullptr);
        m_dense.clear();
    }

    std::size_t Size() const
    {
        return m_sparse.size();
    }

    std::size_t Count() const
    {
        return m_dense.size();
    }

    bool Empty() const
    {
        return m_dense.empty();
    }

    iterator Begin() const
    {
        return m_dense.begin();
    }

    iterator End() const
    {
        return m_dense.end();
    }

    reverse_iterator Rbegin() const
    {
        return m_dense.rbegin();
    }

    reverse_iterator Rend() const
    {
        return m_dense.rend();
    }

    iterator LowerBound(value_type i) const
    {
        return std::ranges::lower_bound(m_dense.begin(), m_dense.end(), i);
    }

    iterator UpperBound(value_type i) const
    {
        return std::ranges::upper_bound(m_dense.begin(), m_dense.end(), i);
    }
};

///
/// The bounded set is very close to boost::dynamic_bitset
/// It performs practically with the same speed
/// In terms of functinality, it has different member functions (which are similar to those in std::set) and provides an iterator
/// The speed is similar to that of the sparse set, but the repeated iterations over the same set of values are slower.
/// It also uses less memory than sparse set: there is no memory allocation for a vector of values, which is need for the sparse set iterator
///
class BoundedSet
{
private:
    static constexpr std::size_t EmptyIndex = static_cast<std::size_t>(-1);
    typedef std::size_t base_type;
    static constexpr unsigned unsigned_bits = std::numeric_limits<base_type>::digits;
    static constexpr unsigned unsigned_bits_log2 =
        (unsigned_bits == 64 ? 6 : unsigned_bits == 32 ? 5 : unsigned_bits == 16 ? 4 : 3);
    static constexpr unsigned unsigned_bits_log2_mask = 0xFFFFFFFF >> (32 - unsigned_bits_log2);
    static constexpr base_type one_bit = 1;

    unsigned m_size;
    std::vector<base_type> m_bit_array;

public:
    typedef std::size_t value_type;
    typedef std::size_t key_type;
    typedef std::size_t size_type;

    BoundedSet(std::size_t size) :m_size(size), m_bit_array((m_size + unsigned_bits - 1) / unsigned_bits) {}

    BoundedSet() : m_size{0}, m_bit_array{0} {}

    void Swap(BoundedSet& s)
    {
        m_bit_array.swap(s.m_bit_array);
        std::swap(m_size, s.m_size);
    }

    void Resize(std::size_t size)
    {
        m_size = size;
        m_bit_array.resize((m_size + unsigned_bits - 1) / unsigned_bits);
    }

    bool Insert(std::size_t i)
    {
        base_type& v = m_bit_array[i >> unsigned_bits_log2];
        base_type x = v;
        v |= (one_bit << (i & unsigned_bits_log2_mask));

        return x != v;
    }

    void Erase(std::size_t i)
    {
        m_bit_array[i >> unsigned_bits_log2] &= ~(one_bit << (i & unsigned_bits_log2_mask));
    }

    bool Test(std::size_t i) const
    {
        return ((m_bit_array[i >> unsigned_bits_log2] >> (i & unsigned_bits_log2_mask)) & 1) != 0;
    }

    struct iterator
    {
        friend BoundedSet;
        typedef std::forward_iterator_tag iterator_category;
        typedef unsigned value_type;
        typedef std::ptrdiff_t  difference_type;

    private:
        bool Test() const { return ((m_bitArray[m_slotIndex] >> m_bitIndex) & 1) != 0; }

        bool Next()
        {
            m_currentSlot >>= 1;

            if (m_currentSlot == 0)
            {
                m_bitIndex = 0;
                ++m_slotIndex;

                while (m_slotIndex < m_bitArraySize && (m_currentSlot = m_bitArray[m_slotIndex]) == 0)
                {
                    ++m_slotIndex;
                }

                if (m_currentSlot == 0)
                {
                    m_slotIndex = EmptyIndex;
                    m_bitIndex = 0;
                    m_currentSlot = 0;
                    return false;
                }
            }
            else
            {
                ++m_bitIndex;
                if (m_slotIndex == m_bitArraySize - 1 && m_bitIndex > m_lastBit)
                {
                    m_slotIndex = EmptyIndex;
                    m_bitIndex = 0;
                    m_currentSlot = 0;
                    return false;
                }
            }

            unsigned shift = lux::bits::LeastSigBit(m_currentSlot);
            m_currentSlot >>= shift;
            m_bitIndex += shift;

            return true;
        }

        iterator(unsigned size, const std::vector<base_type>&  bit_array, std::size_t pos)
            : m_size(size), m_bitArraySize(bit_array.size()), m_bitArray(size ? &bit_array[0] : nullptr),
            m_slotIndex(pos >> unsigned_bits_log2), m_currentSlot(0), m_bitIndex(pos & unsigned_bits_log2_mask),
            m_lastBit((size - 1) &  unsigned_bits_log2_mask)
        {
            if (pos < m_size)
                m_currentSlot = m_bitArray[m_slotIndex] >> m_bitIndex;

            if (!Test())
                Next();
        }

    public:

        iterator(unsigned size, const std::vector<base_type>&  bit_array)
            : m_size(size), m_bitArraySize(bit_array.size()), m_bitArray(size ? &bit_array[0] : nullptr), m_slotIndex(0), m_currentSlot(0), m_bitIndex(0),
            m_lastBit((size - 1) &  unsigned_bits_log2_mask)
        {
            if (m_size != 0)
                m_currentSlot = m_bitArray[0];

            if (m_size != 0 && !Test())
                Next();
        }

        iterator() : m_size{0}, m_bitArray{nullptr}, m_slotIndex{EmptyIndex}, m_currentSlot{0}, m_bitIndex{0},
            m_lastBit{0}, m_bitArraySize{0} {}

        unsigned operator*() const { return (m_slotIndex << unsigned_bits_log2) + m_bitIndex; }

        iterator& operator++()
        {
            Next();
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp(*this);
            ++*this;
            return tmp;
        }

        bool operator==(const iterator& y) const
        {
            return (m_slotIndex == y.m_slotIndex) && (m_bitIndex == y.m_bitIndex);
        }

        bool operator!=(const iterator& y) const
        {
            return (m_slotIndex != y.m_slotIndex) || (m_bitIndex != y.m_bitIndex);
        }

    private:
        unsigned m_size;
        std::size_t m_bitArraySize;
        const base_type* m_bitArray;
        std::size_t m_slotIndex;
        base_type m_currentSlot;
        unsigned m_bitIndex;
        unsigned m_lastBit;
    };

    typedef iterator const_iterator;

    iterator Begin() const { return iterator(m_size, m_bit_array); }

    iterator End() const { return iterator(); }

    iterator Find(unsigned i) const
    {
        if (Test(i))
            return iterator(m_size, m_bit_array, i);

        return iterator();
    }

    iterator LowerBound(std::size_t i) const { return iterator(m_size, m_bit_array, i); }

    iterator UpperBound(std::size_t i) const
    {
        if (i + 1 >= m_size)
            return iterator();

        return iterator(m_size, m_bit_array, i+1);
    }

    void Erase(const iterator& it) { Erase(*it); }

    bool IsEmpty() const
    {
        for (auto x : m_bit_array)
            if (x != 0)
                return false;

        return true;
    }

    void Clear() { std::ranges::fill(m_bit_array.begin(), m_bit_array.end(), 0); }

    std::size_t Size() const { return m_size; }

    std::size_t Count() const
    {
        std::size_t count = 0;
        for (auto x : m_bit_array)
            count += lux::bits::CountBits(x);

        return count;
    }
};
