/// @file FuraiEngine/Collections/Array.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito.
/// 配列を提供します。
#ifndef _FURAIENGINE_COLLECTIONS_ARRAY_HPP
#define _FURAIENGINE_COLLECTIONS_ARRAY_HPP
#include <initializer_list>
#include <iterator>
#include "FuraiEngine/Memory.hpp"
/// FuraiEngineのすべての機能を含む名前空間です。
namespace FuraiEngine
{
    /// ポインタイテレータです。
    template<typename T>
    class PointerIterator
    {
        T *m_pElement; // 現在位置の要素です。

    public:
        /// イテレータのカテゴリフラグ型です。
        using iterator_category = std::random_access_iterator_tag;

        /// 要素型の型エイリアスです。
        using value_type = T;

        /// イテレータの移動距離を表現する為の符号付き整数型の型エイリアスです。
        using difference_type = ISize;

        /// 要素型のポインタ型エイリアスです。
        using pointer = T *;

        /// 要素型の参照型エイリアスです。
        using reference = T &;

        /// 初期化します。
        /// @param pointer メモリ上の現在地を指すポインタです。
        PointerIterator(T *pointer) noexcept
            : m_pElement(pointer)
        {}

        /// コピーします。
        /// @param origin コピー元です。
        PointerIterator(const PointerIterator<T> &origin) noexcept
            : m_pElement(origin.m_pElement)
        {}

        /// ムーブします。
        /// @param origin ムーブ元です。
        PointerIterator(PointerIterator<T> &&origin) noexcept
            : m_pElement(origin.m_pElement)
        {
            origin.m_pElement = nullptr;
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        PointerIterator<T> &
        operator=(const PointerIterator<T> &origin) noexcept
        {
            this->m_pElement = origin.m_pElement;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        PointerIterator<T> &operator=(PointerIterator<T> &&origin) noexcept
        {
            this->m_pElement  = origin.m_pElement;
            origin.m_pElement = nullptr;
            return *this;
        }

        /// 指定分進めます。
        /// @param step 進める数です。
        PointerIterator<T> &operator+=(USize step) noexcept
        {
            this->m_pElement += step;
            return *this;
        }

        /// 指定分戻ります。
        /// @param step 戻る数です。
        PointerIterator<T> &operator-=(USize step) noexcept
        {
            this->m_pElement -= step;
            return *this;
        }

        /// 一つ進めます。
        PointerIterator<T> &operator++() noexcept
        {
            this->m_pElement += 1;
            return *this;
        }

        /// 一つ進めて、進む前のイテレータを返します。
        /// @return 進める前のイテレータです。
        PointerIterator<T> operator++(int) noexcept
        {
            auto iter = *this;
            this->m_pElement += 1;
            return iter;
        }

        /// 一つ戻ります。
        PointerIterator<T> &operator--() noexcept
        {
            this->m_pElement -= 1;
            return *this;
        }

        /// 一つ戻って、戻る前のイテレータを返します。
        /// @return 戻る前のイテレータです。
        PointerIterator<T> operator--(int) noexcept
        {
            auto iter = *this;
            this->m_pElement -= 1;
            return iter;
        }

        /// 要素にアクセスします。
        /// @return 要素の参照です。
        /// @warning 参照先がヌルの場合、異常終了します。
        T &operator*() noexcept
        {
            if (this->m_pElement != nullptr)
                return *this->m_pElement;
            else
            {
                _Internal::Logger(_Internal::ERROR_LABEL)
                    .Write("参照先がnullでした。")
                    .Write("'")
                    .Write(typeid(T).name())
                    .Write(" &PointerIterator<")
                    .Write(typeid(T).name())
                    .Write(">::operator*() noexcept'");

                ExitError();
            }
        }

        /// 要素にアクセスします。
        /// @return 要素の参照です。
        /// @warning 参照先がヌルの場合、異常終了します。
        const T &operator*() const
        {
            if (this->m_pElement != nullptr)
                return *this->m_pElement;
            else
            {
                _Internal::Logger(_Internal::ERROR_LABEL)
                    .Write("参照先がnullでした。")
                    .Write("'const ")
                    .Write(typeid(T).name())
                    .Write(" &PointerIterator<")
                    .Write(typeid(T).name())
                    .Write(">::operator*() noexcept'");

                ExitError();
            }
        }

        /// 要素が同等か比較します。
        /// @param other 比較対象です。
        /// @return 同等の場合、真です。
        Bool operator==(const PointerIterator<T> &other) const noexcept
        {
            return l.m_pElement == r.m_pElement;
        }

        /// 要素が不等か比較します。
        /// @param other 比較対象です。
        /// @return 不等の場合、真です。
        Bool operator!=(const PointerIterator<T> &other) const noexcept
        {
            return l.m_pElement != r.m_pElement;
        }
    };

    /// 不変ポインタイテレータです。
    template<typename T>
    class ConstPointerIterator
    {
        T *m_pElement; // 現在位置の要素です。

    public:
        /// イテレータのカテゴリフラグ型です。
        using iterator_category = std::random_access_iterator_tag;

        /// 要素型の型エイリアスです。
        using value_type = const T;

        /// イテレータの移動距離を表現する為の符号付き整数型の型エイリアスです。
        using difference_type = ISize;

        /// 要素型のポインタ型エイリアスです。
        using pointer = const T *;

        /// 要素型の参照型エイリアスです。
        using reference = const T &;

        /// 初期化します。
        /// @param pointer メモリ上の現在地を指すポインタです。
        ConstPointerIterator(T *pointer) noexcept
            : m_pElement(pointer)
        {}

        /// コピーします。
        /// @param origin コピー元です。
        ConstPointerIterator(const ConstPointerIterator<T> &origin) noexcept
            : m_pElement(origin.m_pElement)
        {}

        /// ムーブします。
        /// @param origin ムーブ元です。
        ConstPointerIterator(ConstPointerIterator<T> &&origin) noexcept
            : m_pElement(origin.m_pElement)
        {
            origin.m_pElement = nullptr;
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        ConstPointerIterator<T> &
        operator=(const ConstPointerIterator<T> &origin) noexcept
        {
            this->m_pElement = origin.m_pElement;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        ConstPointerIterator<T> &
        operator=(ConstPointerIterator<T> &&origin) noexcept
        {
            this->m_pElement  = origin.m_pElement;
            origin.m_pElement = nullptr;
            return *this;
        }

        /// 指定分進めます。
        /// @param step 進める数です。
        ConstPointerIterator<T> &operator+=(USize step) noexcept
        {
            this->m_pElement += step;
            return *this;
        }

        /// 指定分戻ります。
        /// @param step 戻る数です。
        ConstPointerIterator<T> &operator-=(USize step) noexcept
        {
            this->m_pElement -= step;
            return *this;
        }

        /// 一つ進めます。
        ConstPointerIterator<T> &operator++() noexcept
        {
            this->m_pElement += 1;
            return *this;
        }

        /// 一つ進めて、進む前のイテレータを返します。
        /// @return 進める前のイテレータです。
        ConstPointerIterator<T> operator++(int) noexcept
        {
            auto iter = *this;
            this->m_pElement += 1;
            return iter;
        }

        /// 一つ戻ります。
        ConstPointerIterator<T> &operator--() noexcept
        {
            this->m_pElement -= 1;
            return *this;
        }

        /// 一つ戻って、戻る前のイテレータを返します。
        /// @return 戻る前のイテレータです。
        ConstPointerIterator<T> operator--(int) noexcept
        {
            auto iter = *this;
            this->m_pElement -= 1;
            return iter;
        }

        /// 要素にアクセスします。
        /// @return 要素の参照です。
        const T &operator*() const
        {
            if (this->m_pElement != nullptr)
                return *this->m_pElement;
            else
            {
                _Internal::Logger(_Internal::ERROR_LABEL)
                    .Write("参照先がnullでした。")
                    .Write("'const ")
                    .Write(typeid(T).name())
                    .Write(" &ConstPointerIterator<")
                    .Write(typeid(T).name())
                    .Write(">::operator*() noexcept'");

                ExitError();
            }
        }

        /// 要素が同等か比較します。
        /// @param other 比較対象です。
        /// @return 同等の場合、真です。
        Bool operator==(const ConstPointerIterator<T> &other) const noexcept
        {
            return l.m_pElement == r.m_pElement;
        }

        /// 要素が不等か比較します。
        /// @param other 比較対象です。
        /// @return 不等の場合、真です。
        Bool operator!=(const ConstPointerIterator<T> &other) const noexcept
        {
            return l.m_pElement != r.m_pElement;
        }
    };

    /// 動的配列です。
    /// @tparam T 要素の型です。
    /// @tparam A アロケータの型です。
    template<typename T, typename A = Allocator<T>>
    class Array
    {
        static constexpr USize ARRAY_SIZE_MIN = 8; // 配列長の最小サイズです。

    public:
        /// 要素の型です。
        using ElementType = T;
        /// アロケータの型です。
        using AllocatorType = A;
        /// 可変イテレータの型です。
        using IteratorType = PointerIterator<ElementType>;
        /// 不変イテレータの型です。
        using ConstIteratorType = ConstPointerIterator<ElementType>;

    private:
        USize         m_arraySize;
        USize         m_elementsCount;
        AllocatorType m_allocator;
        ElementType  *m_pArray;

    public:
        /// 配列長を指定して初期化します。
        /// @param size 配列長です。
        /// @param allocator アロケータです。
        /// @warning メモリ確保に失敗した場合、異常終了します。
        Array(USize size, const AllocatorType &allocator = AllocatorType())
            : m_arraySize((size < ARRAY_SIZE_MIN ? ARRAY_SIZE_MIN : size))
            , m_elementsCount(0)
            , m_allocator(allocator)
            , m_pArray(nullptr)
        {
            if (!this->m_allocator
                     .Allocate(this->m_arraySize)
                     .IsSuccess(this->m_pArray))
            {
                _Internal::Logger(_Internal::ERROR_LABEL)
                    .Write("メモリの確保に失敗しました。")
                    .Write("'Array<")
                    .Write(TypenameOf<T>())
                    .Write(">::Array(USize size, const ")
                    .Write(TypenameOf<AllocatorType>())
                    .Write(" &allocator) noexcept'");

                ExitError();
            }
        }

        /// 初期化します。
        /// @param allocator アロケータです。
        /// @warning メモリ確保に失敗した場合、異常終了します。
        Array(const AllocatorType &allocator = AllocatorType()) noexcept
            : m_arraySize(ARRAY_SIZE_MIN)
            , m_elementsCount(0)
            , m_allocator(allocator)
            , m_pArray(nullptr)
        {
            if (!this->m_allocator
                     .Allocate(this->m_arraySize)
                     .IsSuccess(this->m_pArray))
            {
                _Internal::Logger(_Internal::ERROR_LABEL)
                    .Write("メモリの確保に失敗しました。")
                    .Write("'Array<")
                    .Write(TypenameOf<T>())
                    .Write(">::Array(const ")
                    .Write(TypenameOf<AllocatorType>())
                    .Write(" &allocator) noexcept'");

                ExitError();
            }
        }

        /// 初期化リストで初期化します。
        /// @param list 初期化リストです。
        /// @param allocator アロケータです。
        /// @warning メモリ確保に失敗した場合、異常終了します。
        Array(std::initializer_list<T> &list,
              const AllocatorType &allocator = AllocatorType())
            : m_arraySize(
                (list.size() < ARRAY_SIZE_MIN ? ARRAY_SIZE_MIN : list.size()))
            , m_elementsCount(0)
            , m_allocator(allocator)
            , m_pArray(nullptr)
        {
            if (!this->m_allocator
                     .Allocate(this->m_arraySize)
                     .IsSuccess(this->m_pArray))
            {
                _Internal::Logger(_Internal::ERROR_LABEL)
                    .Write("メモリの確保に失敗しました。")
                    .Write("'Array<")
                    .Write(TypenameOf<T>())
                    .Write(
                        ">::Array(std::initializer_list "
                        "&list, const ")
                    .Write(TypenameOf<AllocatorType>())
                    .Write(" &allocator) noexcept'");

                ExitError();
            }

            // 挿入します。
            USize i = 0;
            for (auto itr = list.begin(); itr != list.end(); ++itr)
            {
                this->m_pArray[i] = *itr;
                i += 1;
            }
        }
    };
}
#endif // !_FURAIENGINE_COLLECTIONS_ARRAY_HPP