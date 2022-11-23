/// @file FuraiEngine/Set.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// 集合を提供します。
#ifndef _FURAIENGINE_SET_HPP
#define _FURAIENGINE_SET_HPP

#include "FuraiEngine/Array.hpp"

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    /// @cond FURAIDOC_INTERNAL
    template <typename K, typename V, typename H, typename E, typename PA, typename IA>
    class Map;
    /// @endcond

    /// ハッシュ関数オブジェクトの特殊化構造体です。
    /// @tparam T ハッシュ値を計算する値の型です。
    template <typename T>
    struct HashOf
    {
        /// ハッシュ値を取得します。
        /// @param value ハッシュ値を計算する値です。
        /// @return ハッシュ値です。
        USize operator()(const T &value) const noexcept
        {
            static_assert(false, "No specializations of this type exist by HashOf.");

            return 0;
        }
    };

    /// ハッシュ値を取得します。
    /// @tparam T ハッシュ値を計算する値の型です。
    /// @param value ハッシュ値を計算する値です。
    /// @return ハッシュ値です。
    template <typename T>
    USize hashOf(const T &value) noexcept
    {
        return HashOf<T>(value);
    }

    /// ハッシュ値を加算します。
    /// @param hash 加算先です。
    /// @param value 加算対象です。
    void addHash(USize &hash, U8 value) noexcept;

    /// ハッシュ値を加算します。
    /// @param hash 加算先です。
    /// @param value 加算対象です。
    void addHash(USize &hash, U16 value) noexcept;

    /// ハッシュ値を加算します。
    /// @param hash 加算先です。
    /// @param value 加算対象です。
    void addHash(USize &hash, U32 value) noexcept;

    /// ハッシュ値を加算します。
    /// @param hash 加算先です。
    /// @param value 加算対象です。
    void addHash(USize &hash, U64 value) noexcept;

#define HASH_OF(T)                                       \
    template <>                                          \
    struct HashOf<T>                                     \
    {                                                    \
        USize operator()(const I8 &value) const noexcept \
        {                                                \
            return static_cast<USize>(value);            \
        }                                                \
    }

    HASH_OF(I8);
    HASH_OF(U8);
    HASH_OF(I16);
    HASH_OF(U16);
    HASH_OF(I32);
    HASH_OF(U32);
    HASH_OF(I64);
    HASH_OF(U64);
    HASH_OF(F32);
    HASH_OF(F64);
    HASH_OF(Bool);
    HASH_OF(Char);

    /// @cond FURAIDOC_INTERNAL
    /// ハッシュ関数オブジェクトのString特殊化構造体です。
    template <>
    struct HashOf<String>
    {
        /// ハッシュ値を取得します。
        /// @param value ハッシュ値を計算する値です。
        /// @return ハッシュ値です。
        USize operator()(const String &value) const noexcept
        {
            USize hash = 0;
            for (auto ch : value)
            {
                addHash(hash, static_cast<U8>(ch));
            }
            return hash;
        }
    };
    /// @endcond

    /// 等価関数オブジェクトの特殊化構造体です。
    /// @tparam T 比較する値の型です。
    template <typename T>
    struct EqualOf
    {
        /// 等しいか判定します。
        /// @param l 比較する左辺値です。
        /// @param r 比較する右辺値です。
        /// @return 等しい場合は、真を返します。
        Bool operator()(const T &l, const T &r) const noexcept
        {
            return l == r;
        }
    };

    /// 集合型です。
    /// @tparam T 要素の型です。
    /// @tparam H ハッシュ関数オブジェクトの型です。
    /// @tparam E 同等比較関数オブジェクトの型です。
    /// @tparam TA 要素配列用アロケータの型です。
    /// @tparam IA インデックス配列用アロケータです。
    template <typename T, typename H = HashOf<T>, typename E = EqualOf<T>, typename TA = Allocator<T>, typename IA = Allocator<USize>>
    class Set
    {
        template <typename K, typename V, typename H, typename E, typename PA, typename IA>
        friend class Map;

        constexpr F32 THRESHOLD = 0.8f; // 全体の何割が埋まったら拡張するかを表す閾値です。

        Array<T, TA> m_elements;    // 要素配列です。
        Array<USize, IA> m_indices; // 添え字配列です。
        H m_hashOf;                 // ハッシュ関数オブジェクトです。
        E m_equalOf;                // 同等比較関数オブジェクトです。

        /// ハッシュ値から位置を計算します。
        /// @param hash ハッシュ値です。
        /// @return 計算した位置です。
        USize indexByHash(USize hash) const noexcept
        {
            return hash % this->m_indices.count();
        }

        /// ハッシュ値から計算した位置に要素の位置を挿入します。
        /// @param hash ハッシュ値です。
        /// @param position 要素の位置です。
        void insertIndexByHash(USize hash, USize position) noexcept
        {
            auto index = this->indexByHash(hash);
            for (USize i = index; i < this->m_indices.count(); i++)
            {
                if (this->m_indices[i] == USIZE_MAX)
                {
                    this->m_indices[i] = position;
                    return;
                }
            }
            for (USize i = 0; i < index; i++)
            {
                if (this->m_indices[i] == USIZE_MAX)
                {
                    this->m_indices[i] = position;
                    return;
                }
            }
        }

        /// ハッシュ値と値から位置の場所を取得します。
        /// @param hash ハッシュ値です。
        /// @param value 比較する値です。
        /// @return 位置のインデックスです。
        /// @retval USIZE_MAX 見つかりませんでした。
        USize positionIndexOf(USize hash, const T &value) const noexcept
        {
            auto index = this->indexByHash(hash);
            for (USize i = index; i < this->m_indices.count(); i++)
            {
                if (this->m_equalOf(value, this->m_elements[this->m_indices[i]]))
                {
                    return i;
                }
            }
            for (USize i = 0; i < index; i++)
            {
                if (this->m_equalOf(value, this->m_elements[this->m_indices[i]]))
                {
                    return i;
                }
            }
            return USIZE_MAX;
        }

        /// 挿入幅を2倍に拡張します。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void expandIndices()
        {
            // 初期化します
            auto length = this->m_indices.count();
            for (USize i = 0; i < length; i++)
            {
                this->m_indices[i] = USIZE_MAX;
            }
            // 拡張します
            for (USize i = 0; i < length; i++)
            {
                this->m_indices.push(USIZE_MAX);
            }
            // 登録します
            for (USize i = 0; i < this->m_elements.count(); i++)
            {
                auto hash = this->m_hashOf(this->m_elements[i]);
                this->insertIndexByHash(hash, i);
            }
        }

        /// 挿入幅を半分に縮小します。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void reducedIndices()
        {
            // 縮小します
            for (USize i = 0; i < length; i++)
            {
                this->m_indices.removeAt(this->m_indices.count() - 1);
            }
            // 初期化します
            auto length = this->m_indices.count();
            for (USize i = 0; i < length; i++)
            {
                this->m_indices[i] = USIZE_MAX;
            }
            // 登録します
            for (USize i = 0; i < this->m_elements.count(); i++)
            {
                auto hash = this->m_hashOf(this->m_elements[i]);
                this->insertIndexByHash(hash, i);
            }
        }

        /// ハッシュ値から計算した位置に挿入します。
        /// @param hash ハッシュ値です。
        /// @param value 挿入する値です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void insertByHash(USize hash, const T &value)
        {
            // 閾値を越していた場合、位置配列を拡張します。
            if (this->m_elements.count() > this->m_indices.count() * this->THRESHOLD)
            {
                this->expandIndices();
            }
            // 挿入します。
            auto position = this->m_elements.count();
            this->m_elements.push(value);
            this->insertIndexByHash(hash, position);
        }

        /// 指定位置の値を削除します。
        /// @param index 削除する位置です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void removeByPositionIndex(USize index)
        {
            auto removePosition = this->m_indices[index];
            auto lastPosition = this->m_indices[this->m_indices.count() - 1];
            auto lastHash = this->m_hashOf(this->m_elements[lastPosition]);
            auto lastIndex = this->positionIndexOf(lastHash, this->m_elements[lastPosition]);
            this->m_elements[removePosition] = this->m_elements[lastPosition];
            this->m_elements.removeAt(lastPosition);
            this->m_indices[lastIndex] = removePosition;
            this->m_indices[index] = USIZE_MAX;

            // 半減値の閾値を下回っていた場合、位置配列を縮小します。
            if (this->m_elements.count() < this->m_indices.count() * 0.5f * this->THRESHOLD)
            {
                this->reducedIndices();
            }
        }

        /// ハッシュ値と値で削除します。
        /// @param hash ハッシュ値です。
        /// @param value 削除する値です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void removeByHash(USize hash, const T &value)
        {
            // 削除します。
            auto index = this->positionIndexOf(hash, value);
            if (index != USIZE_MAX)
            {
                this->removeByPositionIndex(index);
            }
        }

    public:
        /// 指定の配列長で初期化します。
        /// @param length 初期の配列長です。
        /// @param hashOf ハッシュ関数オブジェクトです。
        /// @param equalOf 同等比較関数オブジェクトです。
        /// @param elementsAllocator 要素配列用アロケータです。
        /// @param indicesAllocator インデックス配列用アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Set(
            USize length,
            const H &hashOf = H(),
            const E &equalOf = E(),
            const TA &elementsAllocator = TA(),
            const IA &indicesAllocator = IA())
            : m_elements(length, elementsAllocator), m_indices((length < 8 ? 8 : length) * 2, indicesAllocator), m_hashOf(hashOf), m_equalOf(equalOf)
        {
        }

        /// 空の配列で初期化します。
        /// @param hashOf ハッシュ関数オブジェクトです。
        /// @param equalOf 同等比較関数オブジェクトです。
        /// @param elementsAllocator 要素配列用アロケータです。
        /// @param indicesAllocator インデックス配列用アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Set(
            const H &hashOf = H(),
            const E &equalOf = E(),
            const TA &elementsAllocator = TA(),
            const IA &indicesAllocator = IA())
            : Set(0, hashOf, equalOf, elementsAllocator, indicesAllocator)
        {
        }

        /// 初期要素を指定して初期化します。
        /// @param list 初期化リストです。
        /// @param hashOf ハッシュ関数オブジェクトです。
        /// @param equalOf 同等比較関数オブジェクトです。
        /// @param elementsAllocator 要素配列用アロケータです。
        /// @param indicesAllocator インデックス配列用アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Set(
            std::initializer_list<T> &list,
            const H &hashOf = H(),
            const E &equalOf = E(),
            const TA &elementsAllocator = TA(),
            const IA &indicesAllocator = IA())
            : m_elements(list, elementsAllocator), m_indices((list.size() < 8 ? 8 : list.size()) * 2, indicesAllocator), m_hashOf(hashOf), m_equalOf(equalOf)
        {
            for (auto elem : this->m_elements)
            {
                auto hash = this->m_hashOf(elem);
                this->insertByHash(hash, elem);
            }
        }

        /// コピー代入します。
        /// @param origin コピー代入元です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Set<T, H, E, TA, IA> &operator=(const Set<T, H, E, TA, IA> &origin)
        {
            this->m_elements = origin.m_elements;
            this->m_indices = origin.m_indices;
            this->m_hashOf = origin.m_hashOf;
            this->m_equalOf = origin.m_equalOf;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ代入元です。
        Set<T, H, E, TA, IA> &operator=(Set<T, H, E, TA, IA> &&origin) noexcept
        {
            // 自身をムーブしていない場合に実行します。
            if (this != &origin)
            {
                this->m_elements = std::move(origin.m_elements);
                this->m_indices = std::move(origin.m_indices);
                this->m_hashOf = std::move(origin.m_hashOf);
                this->m_equalOf = std::move(origin.m_equalOf);
            }
            return *this;
        }

        /// コピーします。
        /// @param origin コピー元です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Set(const Set<T, H, E, TA, IA> &origin)
        {
            *this = origin;
        }

        /// ムーブします。
        /// @param origin ムーブ元です。
        Set(Set<T, H, E, TA, IA> &&origin) noexcept
        {
            *this = std::move(origin);
        }

        /// 要素を追加します。
        /// @param value 追加する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void insert(const T &value)
        {
            auto hash = this->m_hashOf(value);
            this->insertByHash(hash, value);
        }

        /// 要素を追加します。
        /// @param value 追加する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void insert(T &&value)
        {
            auto hash = this->m_hashOf(value);
            this->insertByHash(hash, value);
        }

        /// 要素を削除します。
        /// @param value 削除する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void remove(const T &value)
        {
            auto hash = this->m_hashOf(value);
            this->removeByHash(hash, value);
        }

        /// 要素を削除します。
        /// @param value 削除する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void remove(T &&value)
        {
            auto hash = this->m_hashOf(value);
            this->removeByHash(hash, value);
        }

        /// 要素が含まれているか判定します。
        /// @param value 判定する要素です。
        Bool contains(const T &value) const noexcept
        {
            auto hash = this->m_hashOf(value);
            return USIZE_MAX != this->positionIndexOf(hash, value);
        }

        /// 要素が含まれているか判定します。
        /// @param value 判定する要素です。
        Bool contains(T &&value) const noexcept
        {
            auto hash = this->m_hashOf(value);
            return USIZE_MAX != this->positionIndexOf(hash, value);
        }

        /// 要素数を取得します。
        /// @return 要素数です。
        USize count() const noexcept
        {
            return this->m_elements.count();
        }

        /// 先頭の可変イテレータを取得します。
        /// @return 先頭イテレータです。
        PointerIterator<T> begin() noexcept
        {
            return this->m_elements.begin();
        }

        /// 先頭の不変イテレータを取得します。
        /// @return 先頭イテレータです。
        ConstPointerIterator<T> begin() const noexcept
        {
            return this->m_elements.begin();
        }

        /// 番兵の可変イテレータを取得します。
        /// @return 番兵イテレータです。
        PointerIterator<T> end() noexcept
        {
            return this->m_elements.end();
        }

        /// 番兵の不変イテレータを取得します。
        /// @return 番兵イテレータです。
        ConstPointerIterator<T> end() const noexcept
        {
            return this->m_elements.end();
        }
    };
}
#endif // !_FURAIENGINE_SET_HPP