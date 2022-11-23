/// @file FuraiEngine/Array.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// 配列を提供します。
#ifndef _FURAIENGINE_ARRAY_HPP
#define _FURAIENGINE_ARRAY_HPP

#include <initializer_list>
#include "FuraiEngine/String.hpp"
#include "FuraiEngine/Errors.hpp"
#include "FuraiEngine/Allocation.hpp"
#include "FuraiEngine/Functional.hpp"

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    /// 標準配列です。
    /// @tparam T 要素の型です。
    /// @tparam A アロケータの型です。
    template<typename T, typename A = Allocator<T>>
    class Array
    {
        static constexpr USize INIT_LENGTH = 8; // 初期の配列長です。

        USize m_length; // 配列長です。
        USize m_count;  // 要素数です。
        A m_allocator;  // アロケータです。
        T *m_buffer;    // 要素のバッファです。

        /// 要素を別バッファに移動させます。
        /// @param oldBuffer 移動元のバッファです。
        /// @param fromIndex 移動元の開始位置です。
        /// @param newBuffer 移動先のバッファです。
        /// @param toIndex 移動先の開始位置です。
        /// @param count 要素数です。
        void moveElements(T *oldBuffer, USize fromIndex, T *newBuffer, USize toIndex, USize count) noexcept
        {
            for (USize i = toIndex, j = fromIndex; i < count; i++, j++)
            {
                newBuffer[i] = std::move(oldBuffer[j]);
            }
        }

        /// 後ろから順に要素を別バッファに移動させます。
        /// @param oldBuffer 移動元のバッファです。
        /// @param fromIndex 移動元の開始位置です。
        /// @param newBuffer 移動先のバッファです。
        /// @param toIndex 移動先の開始位置です。
        /// @param count 要素数です。
        void moveElementsRev(T *oldBuffer, USize fromIndex, T *newBuffer, USize toIndex, USize count) noexcept
        {
            for (ISize i = toIndex + count - 1, j = fromIndex + count - 1; i >= toIndex; i--, j--)
            {
                newBuffer[i] = std::move(oldBuffer[j]);
            } 
        }

        /// バッファを2倍に拡張します。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void expandBuffer()
        {
            // 作成します。
            USize newLength = this->m_length * 2;
            auto newBuffer = this->m_allocator.allocate(newLength);
            // 移動します。
            this->moveElements(this->m_buffer, 0, newBuffer, 0, this->m_count);
            // 古いデータを削除します。
            this->m_allocator.deallocate(this->m_buffer, this->m_length);
            // 上書きします。
            this->m_length = newLength;
            this->m_buffer = newBuffer;
        }

        /// バッファを半分に縮小します。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void reducedBuffer()
        {
            // 作成します。
            USize newLength = static_cast<USize>(this->m_length * 0.5f);
            auto newBuffer = this->m_allocator.allocate(newLength);
            // 移動します。
            this->moveElements(this->m_buffer, 0, newBuffer, 0, this->m_count);
            // 古いデータを削除します。
            this->m_allocator.deallocate(this->m_buffer, this->m_length);
            // 上書きします。
            this->m_length = newLength;
            this->m_buffer = newBuffer;
        }

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
        Array<T, A> &operator=(const Array<T, A> &origin)
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
        Array<T, A> &operator=(Array<T, A> &&origin) noexcept
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
        Array(const Array<T, A> &origin)
        {
            *this = origin;
        }

        /// ムーブします。
        /// @param origin ムーブ元です。
        Array(Array<T, A> &&origin) noexcept
        {
            *this = std::move(origin);
        }

        /// 解体します。
        ~Array()
        {
            if (this->m_buffer)
            {
                this->m_allocator.deallocate(this->m_buffer, this->m_length);
            }
        }

        /// 添え字からアクセスします。
        /// @param index アクセスする位置を指す添え字です。
        /// @return 要素の参照です。
        /// @exception BufferOverException 指定位置は配列の範囲外の可能性があります。
        T &operator[](USize index)
        {
            if (index >= this->m_count) throw BufferOverException(TXT("指定位置は配列の範囲外です。"));

            return this->m_buffer[index];
        }

        /// 添え字からアクセスします。
        /// @param index アクセスする位置を指す添え字です。
        /// @return 要素の参照です。
        /// @exception BufferOverException 指定位置は配列の範囲外の可能性があります。
        const T &operator[](USize index) const
        {
            if (index >= this->m_count) throw BufferOverException(TXT("指定位置は配列の範囲外です。"));

            return this->m_buffer[index];
        }

        /// 添え字からアクセスします。
        /// @param index アクセスする位置を指す添え字です。
        /// @return 要素の参照です。
        /// @exception BufferOverException 指定位置は配列の範囲外の可能性があります。
        T &at(USize index)
        {
            return (*this)[index];
        }

        /// 添え字からアクセスします。
        /// @param index アクセスする位置を指す添え字です。
        /// @return 要素の参照です。
        /// @exception BufferOverException 指定位置は配列の範囲外の可能性があります。
        const T &at(USize index) const
        {
            return (*this)[index];
        }

        /// 末尾に要素を追加します。
        /// @param value 追加要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void push(const T &value)
        {
            // 配列長と要素数が一致した場合、バッファを拡張します。
            if (this->m_count == this->m_length)
            {
                this->expandBuffer();
            }

            // 追加します。
            this->m_buffer[this->m_count] = value;
            this->m_count += 1;
        }

        /// 末尾に要素を追加します。
        /// @param value 追加要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void push(T &&value)
        {
            // 配列長と要素数が一致した場合、バッファを拡張します。
            if (this->m_count == this->m_length)
            {
                this->expandBuffer();
            }

            // 追加します。
            this->m_buffer[this->m_count] = std::move(value);
            this->m_count += 1;
        }

        /// 指定位置に要素を挿入します。
        /// @param index 追加位置です。
        /// @param value 追加要素です。
        /// @exception BufferOverException 追加位置が配列の範囲外の可能性があります。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void insert(USize index, const T &value)
        {
            if (index < this->m_count) throw BufferOverException(TXT("追加位置が配列の範囲外です。"));

            // 配列長と要素数が一致した場合、バッファを拡張します。
            if (this->m_count == this->m_length)
            {
                this->expandBuffer();
            }

            // 要素を移動させます。
            this->moveElementsRev(this->m_buffer, index, this->m_buffer, index + 1, this->m_count);

            // 追加します。
            this->m_buffer[index] = value;
            this->m_count += 1;
        }
        
        /// 指定位置に要素を挿入します。
        /// @param index 追加位置です。
        /// @param value 追加要素です。
        /// @exception BufferOverException 追加位置が配列の範囲外の可能性があります。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void insert(USize index, T &&value)
        {
            if (index < this->m_count) throw BufferOverException(TXT("追加位置が配列の範囲外です。"));

            // 配列長と要素数が一致した場合、バッファを拡張します。
            if (this->m_count == this->m_length)
            {
                this->expandBuffer();
            }

            // 要素を移動させます。
            this->moveElementsRev(this->m_buffer, index, this->m_buffer, index + 1, this->m_count);

            // 追加します。
            this->m_buffer[index] = std::move(value);
            this->m_count += 1;
        }

        /// 指定位置の要素を削除します。
        /// @param index 削除位置です。
        /// @exception BufferOverException 追加位置が配列の範囲外の可能性があります。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void removeAt(USize index)
        {
            if (index < this->m_count) throw BufferOverException(TXT("削除位置が配列の範囲外です。"));

            // 要素を移動させます。
            this->moveElements(this->m_buffer, index + 1, this->m_buffer, index, this->m_count);
            this->m_count -= 1;

            // 要素数が配列長の半分未満の場合、バッファを縮小します。
            if (this->m_count < static_cast<USize>(this->m_length * 0.5f))
            {
                this->reducedBuffer();
            }
        }

        /// はじめに見つかった要素の位置を取得します。
        /// @param value 捜索対象の要素です。
        /// @param equal 等価を比較する関数です。
        /// @return はじめに見つかった要素の位置です。
        /// @retval USIZE_MAX 見つかりませんでした。
        USize indexOf(const T &value, const Func<Bool(const T &, const T &)> &equal = [](const T &l, const T &r){ return l == r; }) const noexcept
        {
            for (USize i = 0; i < this->m_count; i++)
            {
                if (equal(value, this->m_buffer[i]))
                {
                    return i;
                }
            }
            return USIZE_MAX;
        }

        /// 指定要素を削除します。
        /// @param value 削除する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void remove(const T &value)
        {
            auto index = this->indexOf(value);
            if (index != USIZE_MAX)
            {
                this->removeAt(static_cast<USize>(index));
            }
        }

        /// 末尾から要素を取り出します。
        /// @return 取り出した要素です。
        /// @exception NullReferenceException 要素数が0の可能性があります。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        T &pop()
        {
            auto index = this->m_count;
            if (index != 0)
            {
                auto elem = this->m_buffer[index - 1];
                this->removeAt(index - 1);
                return elem;
            }
            else
            {
                throw NullReferenceException("要素数が0です。");
            }
        }

        /// 要素数を取得します。
        /// @return 要素数です。
        USize count() const noexcept
        {
            return this->m_count;
        }

        /// 先頭の可変イテレータを取得します。
        /// @return 先頭イテレータです。
        PointerIterator<T> begin() noexcept
        {
            return PointerIterator<T>(&this->m_buffer[0]);
        }

        /// 先頭の不変イテレータを取得します。
        /// @return 先頭イテレータです。
        ConstPointerIterator<T> begin() const noexcept
        {
            return ConstPointerIterator<T>(&this->m_buffer[0]);
        }
        
        /// 番兵の可変イテレータを取得します。
        /// @return 番兵イテレータです。
        PointerIterator<T> end() noexcept
        {
            return PointerIterator<T>(&this->m_buffer[this->m_count]);
        }
        
        /// 番兵の不変イテレータを取得します。
        /// @return 番兵イテレータです。
        ConstPointerIterator<T> end() const noexcept
        {
            return ConstPointerIterator<T>(&this->m_buffer[this->m_count]);
        }
    };
}

#endif // !_FURAIENGINE_ARRAY_HPP