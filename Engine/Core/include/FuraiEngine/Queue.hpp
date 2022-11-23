/// @file FuraiEngine/Queue.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// 待ち行列を提供します。
#ifndef _FURAIENGINE_QUEUE
#define _FURAIENGINE_QUEUE

#include "FuraiEngine/Array.hpp"

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    /// 待ち行列です。
    template<typename T, typename A = Allocator<T>>
    class Queue
    {
        Array<T, A> m_array;
        USize m_count;

    public:

        /// 指定の配列長で初期化します。
        /// @param length 初期の配列長です。
        /// @param allocator アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Queue(USize length, const A &allocator = A())
        : m_array(length, allocator)
        , m_count(0)
        {}

        /// 空の配列を初期化します。
        /// @param allocator アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Queue(const A &allocator = A())
        : m_array(allocator)
        , m_count(0)
        {}

        /// 初期要素を指定して初期化します。
        /// @param list 初期化リストです。
        /// @param allocator アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Queue(std::initializer_list<T> &list, const A &allocator = A())
        : m_array(list, allocator)
        , m_count(list.size())
        {
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Queue<T, A> &operator=(const Queue<T, A> &origin)
        {
            this->m_array = origin.m_array;
            this->m_count = origin.m_count;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        Queue<T, A> &operator=(Queue<T, A> &&origin) noexcept
        {
            // 自身をムーブしていない場合に実行します。
            if (this != &origin)
            {
                this->m_array = std::move(origin.m_array);
                this->m_count = std::move(origin.m_count);
            }
            return *this;
        }

        /// コピーします。
        /// @param origin コピー元です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Queue(const Queue<T, A> &origin)
        {
            *this = origin;
        }

        /// ムーブします。
        /// @param origin ムーブ元です。
        Queue(Queue<T, A> &&origin) noexcept
        {
            *this = std::move(origin);
        }
    };
}
#endif // !_FURAIENGINE_QUEUE