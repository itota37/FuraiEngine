/// @file FuraiEngine/Array.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// 配列を提供します。
#ifndef _FURAIENGINE_ARRAY_HPP
#define _FURAIENGINE_ARRAY_HPP

#include <initializer_list>
#include "FuraiEngine/String.hpp"
#include "FuraiEngine/Allocation.hpp"

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    /// 標準配列です。
    template<typename T, typename A = Allocator<T>>
    class Array
    {
        static constexpr USize INIT_LENGTH = 8; // 初期の配列長です。

        USize m_length; // 配列長です。
        USize m_count;  // 要素数です。
        A m_allocator;  // アロケータです。
        T *m_buffer;    // 要素のバッファです。

    public:

        /// 指定の配列長で初期化します。
        /// @param length 初期の配列長です。
        /// @param allocator アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Array(USize length, const A &allocator = A())
        : m_length(size)
        , m_count(0)
        , m_allocator(allocator)
        , m_buffer(m_allocator.allocate(m_length))
        {}

        /// 空の配列を初期化します。
        /// @param allocator アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Array(const A &allocator = A())
        : Array(INIT_LENGTH, allocator)
        {}

        /// 初期要素を指定して初期化します。
        /// @param list 初期化リストです。
        /// @param allocator アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Array(std::initializer_list<T> &list, const A &allocator = A())
        : Array(list.size() + INIT_LENGTH, allocator)
        , m_count(list.size())
        {
            USize i = 0;
            auto itr = list.begin();
            while (itr != list.end())
            {
                this->m_buffer[i] = *itr;
                i += 1;
                itr++;
            }
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Array<T> &operator=(const Array<T> &origin)
        {
            // 元データを削除します。
            this->m_allocator.deallocate(this->m_buffer, this->m_length);
            // コピーします。
            this->m_length = origin.m_length;
            this->m_count = origin.m_count;
            this->m_allocator = origin.m_allocator;
            this->m_buffer = this->m_allocator.allocate(this->m_length);
            for (USize i = 0; i < this->m_count; i++)
            {
                this->m_buffer[i] = origin.m_buffer[i];
            }
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        Array<T> &operator=(Array<T> &&origin) noexcept
        {
            // 自身をムーブしていない場合に実行します。
            if (this != &origin)
            {
                // 安全性のため、中身を交換します。
                auto tmpLength = this->m_length;
                auto tmpCount = this->m_count;
                auto tmpAllocator = this->m_allocator;
                auto tmpBuffer = this->m_buffer;
                this->m_length = origin.m_length;
                this->m_count = origin.m_count;
                this->m_allocator = origin.m_allocator;
                this->m_buffer = origin.m_buffer;
                origin.m_length = tmpLength;
                origin.m_count = tmpCount;
                origin.m_allocator = tmpAllocator;
                origin.m_buffer = tmpBuffer;
            }
            return *this;
        }

        /// コピーします。
        /// @param origin コピー元です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Array(const Array<T> &origin)
        {
            *this = origin;
        }

        /// ムーブします。
        /// @param origin ムーブ元です。
        Array(Array<T> &&origin) noexcept
        {
            *this = std::move(origin);
        }

        /// 解体します。
        ~Array()
        {
            this->m_allocator.deallocate(this->m_buffer, this->m_length);
        }
    };
}

#endif // !_FURAIENGINE_ARRAY_HPP