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
    /// ハッシュ関数オブジェクトの特殊化構造体です。
    /// @tparam T ハッシュ値を計算する値の型です。
    template<typename T>
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
    template<typename T>
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

#define HASH_OF(T)                                     \
template<> struct HashOf<T> {                          \
    USize operator()(const I8 &value) const noexcept { \
        return static_cast<USize>(value);              \
    }                                                  \
}

    HASH_OF(I8); HASH_OF(U8);
    HASH_OF(I16); HASH_OF(U16);
    HASH_OF(I32); HASH_OF(U32);
    HASH_OF(I64); HASH_OF(U64);
    HASH_OF(F32); HASH_OF(F64);
    HASH_OF(Bool); HASH_OF(Char);

    /// @code FURAIDOC_INTERNAL
    /// ハッシュ関数オブジェクトのString特殊化構造体です。
    template<>
    struct HashOf<String>
    {
        /// ハッシュ値を取得します。
        /// @param value ハッシュ値を計算する値です。
        /// @return ハッシュ値です。
        USize operator()(const String &value) const noexcept
        {
            USize hash = 0;
            for(auto ch: value)
            {
                addHash(hash, static_cast<U8>(ch));
            }
            return hash;
        }
    };
    /// @endcode

    /// 等価関数オブジェクトの特殊化構造体です。
    /// @tparam T 比較する値の型です。
    template<typename T>
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
    /// @tparam A アロケータの型です。
    template<typename T, typename H = HashOf<T>, typename E = EqualOf<T>, typename A = Allocator<T>>
    class Set
    {
        constexpr F32 THRESHOLD = 0.8f; // 全体の何割が埋まったら拡張するかを表す閾値です。

        Array<T, A> m_elements;         // 要素配列です。
        Array<USize, A> m_indices;      // 添え字配列です。
        H m_hashOf;                     // ハッシュ関数オブジェクトです。
        E m_equalOf;                    // 同等比較関数オブジェクトです。

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
                this->m_indices.add(USIZE_MAX);
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
            this->m_elements.add(value);
            this->insertIndexByHash(hash, position);
        }

        /// ハッシュ値と値で削除します。
        /// @param hash ハッシュ値です。
        /// @param value 挿入する値です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void removeByHash(USize hash, const T &value)
        {
            // 削除します。

            // 半減値の閾値を下回っていた場合、位置配列を縮小します。
            if (this->m_elements.count() < this->m_indices.count() * 0.5f * this->THRESHOLD)
            {
                this->reducedIndices();
            }
        }

    public:

        /// 指定の配列長で初期化します。
        /// @param length 初期の配列長です。
        /// @param hashOf ハッシュ関数オブジェクトです。
        /// @param equalOf 同等比較関数オブジェクトです。
        /// @param allocator アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Set(USize length, const H &hashOf = H(), const E &equalOf = E(), const A &allocator = A())
        : m_elements(length, allocator)
        , m_hashOf(hashOf)
        , m_equalOf(equalOf)
        {}

        /// 空の配列で初期化します。
        /// @param hashOf ハッシュ関数オブジェクトです。
        /// @param equalOf 同等比較関数オブジェクトです。
        /// @param allocator アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Set(const H &hashOf = H(), const E &equalOf = E(), const A &allocator = A())
        : Set(0, hashOf, allocator)
        {}

        /// 初期要素を指定して初期化します。
        /// @param list 初期化リストです。
        /// @param hashOf ハッシュ関数オブジェクトです。
        /// @param equalOf 同等比較関数オブジェクトです。
        /// @param allocator アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Set(std::initializer_list<T> &list, const H &hashOf = H(), const E &equalOf = E(), const A &allocator = A())
        : m_elements(list.size() * 2, allocator)
        , m_hashOf(hashOf)
        {}
    };
}
#endif // !_FURAIENGINE_SET_HPP