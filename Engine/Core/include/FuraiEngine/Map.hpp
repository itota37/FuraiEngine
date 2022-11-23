/// @file FuraiEngine/Map.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// 連想配列を提供します。
#ifndef _FURAIENGINE_MAP_HPP
#define _FURAIENGINE_MAP_HPP

#include "FuraiEngine/Set.hpp"

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    /// キーが存在しなかった場合の例外です。
    DYNAMIC_LINK_CORE class KeyNotFoundException : public IException
    {
        String m_message; // エラーメッセージです。

    public:
        /// メッセージを設定して初期化します。
        /// @param message エラーメッセージです。
        KeyNotFoundException(const String &message) noexcept;

        /// コピーします。
        /// @param origin コピー元です。
        KeyNotFoundException(const KeyNotFoundException &origin) noexcept;

        /// ムーブします。
        /// @param origin ムーブ元です。
        KeyNotFoundException(const KeyNotFoundException &origin) noexcept;

        /// コピー代入します。
        /// @param origin コピー元です。
        KeyNotFoundException &operator=(const KeyNotFoundException &origin) noexcept;

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        KeyNotFoundException &operator=(const KeyNotFoundException &origin) noexcept;

        /// エラーメッセージを取得します。
        /// @return エラーメッセージです。
        const Char *message() const noexcept;
    };

    /// キーと値のペア構造です。
    /// @tparam K キーの型です。
    /// @tparam V 値の型です。
    template <typename K, typename V>
    struct Pair
    {
        /// キーです。
        K key;
        /// 値です。
        V value;

        /// 初期化します。
        Pair(const K &key, const V &value)
            : key(key), value(value)
        {
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        Pair<K, V> &operator=(const Pair<K, V> &origin)
        {
            this->key = origin.key;
            this->value = origin.value;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        Pair<K, V> &operator=(Pair<K, V> &&origin) noexcept
        {
            this->key = std::move(origin.key);
            this->value = std::move(origin.value);
            return *this;
        }

        /// コピーします。
        /// @param origin コピー元です。
        Pair(const Pair<K, V> &origin)
        {
            *this = origin;
        }

        /// ムーブします。
        /// @param origin ムーブ元です。
        Pair(Pair<K, V> &&origin) noexcept
        {
            *this = std::move(origin);
        }
    };

    /// @cond FURAIDOC_INTERNAL
    /// キーのハッシュ関数オブジェクトです。
    /// @tparam K キーの型です。
    /// @tparam V 値の型です。
    /// @tparam KH キーのハッシュ関数の型です。
    template <typename K, typename V, typename KH>
    struct HashOfKey
    {
        /// ハッシュ関数です。
        KH hashOf;

        /// 初期化します。
        /// @param hashOf ハッシュ関数です。
        HashOfKey(const KH &hashOf) noexcept
            : hashOf(hashOf)
        {
        }

        /// ハッシュ値を取得します。
        /// @param value ハッシュ値を計算する値です。
        USize operator()(const Pair<K, V> &value) const noexcept
        {
            return this->hashOf(value);
        }
    };

    /// キーの同等比較関数オブジェクトです。
    /// @tparam K キーの型です。
    /// @tparam V 値の型です。
    /// @tparam KE キーの同等比較関数の型です。
    template <typename K, typename V, typename KE>
    struct EqualOfKey
    {
        /// ハッシュ関数です。
        KE equalOf;

        /// 初期化します。
        /// @param equalOf ハッシュ関数です。
        EqualOfKey(const KE &equalOf) noexcept
            : equalOf(equalOf)
        {
        }

        /// キーを比較します。
        /// @param l 左辺値です。
        /// @param r 右辺値です。
        USize operator()(const Pair<K, V> &l, const Pair<K, V> &r) const noexcept
        {
            return this->equalOf(l.key, r.key);
        }
    };
    /// @endcond

    /// 連想配列型です。
    /// @tparam K キーの型です。
    /// @tparam V 値の型です。
    /// @tparam H ハッシュ関数オブジェクトの型です。
    /// @tparam E 同等比較関数オブジェクトの型です。
    /// @tparam PA 要素配列用アロケータの型です。
    /// @tparam IA インデックス配列用アロケータです。
    template <typename K, typename V, typename H = HashOf<K>, typename E = EqualOf<K>, typename PA = Allocator<Pair<K, V>>, typename IA = Allocator<USize>>
    class Map
    {

        Set<Pair<K, V>, HashOfKey<K, V, H>, EqualOfKey<K, V, E>, PA, IA> m_set;
        H m_hashOf;
        E m_equalOf;

        /// キーからペアの位置を取得します。
        /// @param key キーです。
        /// @return ペアの位置です。
        /// @retval USIZE_MAX 一致するキーが見つかりませんでした。
        USize indexOf(const K &key) const noexcept
        {
            auto hash = this->m_hashOf(key);
            auto index = this->m_set.indexByHash(hash);
            for (USize i = index; i < this->m_set.m_elements.count(); i++)
            {
                auto position = this->m_set.m_indices[i];
                if (position == USIZE_MAX) return USIZE_MAX;
                if (this->m_equalOf(key, this->m_set.m_elements[position].key))
                {
                    return i;
                }
            }
            for (USize i = 0; i < index; i++)
            {
                auto position = this->m_set.m_indices[i];
                if (position == USIZE_MAX) return USIZE_MAX;
                if (this->m_equalOf(key, this->m_set.m_elements[position].key))
                {
                    return i;
                }
            }
            return USIZE_MAX;
        }

    public:
        /// 指定の配列長で初期化します。
        /// @param length 初期の配列長です。
        /// @param hashOf ハッシュ関数オブジェクトです。
        /// @param equalOf 同等比較関数オブジェクトです。
        /// @param elementsAllocator 要素配列用アロケータです。
        /// @param indicesAllocator インデックス配列用アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Map(
            USize length,
            const H &hashOf = H(),
            const E &equalOf = E(),
            const PA &elementsAllocator = PA(),
            const IA &indicesAllocator = IA())
            : m_set(length, HashOfKey<K, V, H>(hashOf), EqualOfKey<K, V, E>(equalOf), elementsAllocator, indicesAllocator), m_hashOf(hashOf), m_equalOf(equalOf)
        {
        }

        /// 空の配列で初期化します。
        /// @param hashOf ハッシュ関数オブジェクトです。
        /// @param equalOf 同等比較関数オブジェクトです。
        /// @param elementsAllocator 要素配列用アロケータです。
        /// @param indicesAllocator インデックス配列用アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Map(
            const H &hashOf = H(),
            const E &equalOf = E(),
            const PA &elementsAllocator = PA(),
            const IA &indicesAllocator = IA())
            : m_set(0, HashOfKey<K, V, H>(hashOf), EqualOfKey<K, V, E>(equalOf), elementsAllocator, indicesAllocator), m_hashOf(hashOf), m_equalOf(equalOf)
        {
        }

        /// 初期要素を指定して初期化します。
        /// @param list 初期化リストです。
        /// @param hashOf ハッシュ関数オブジェクトです。
        /// @param equalOf 同等比較関数オブジェクトです。
        /// @param elementsAllocator 要素配列用アロケータです。
        /// @param indicesAllocator インデックス配列用アロケータです。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Map(
            std::initializer_list<Pair<K, V>> &list,
            const H &hashOf = H(),
            const E &equalOf = E(),
            const PA &elementsAllocator = PA(),
            const IA &indicesAllocator = IA())
            : m_set(list, HashOfKey<K, V, H>(hashOf), EqualOfKey<K, V, E>(equalOf), elementsAllocator, indicesAllocator), m_hashOf(hashOf), m_equalOf(equalOf)
        {
        }

        /// コピー代入します。
        /// @param origin コピー代入元です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Map<K, V, H, E, PA, IA> &operator=(const Map<K, V, H, E, PA, IA> &origin)
        {
            this->m_set = origin.m_set;
            this->m_hashOf = origin.m_hashOf;
            this->m_equalOf = origin.m_equalOf;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ代入元です。
        Map<K, V, H, E, TA, IA> &operator=(Map<K, V, H, E, TA, IA> &&origin) noexcept
        {
            // 自身をムーブしていない場合に実行します。
            if (this != &origin)
            {
                this->m_set = std::move(origin.m_set);
                this->m_hashOf = std::move(origin.m_hashOf);
                this->m_equalOf = std::move(origin.m_equalOf);
            }
            return *this;
        }

        /// コピーします。
        /// @param origin コピー元です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Map(const Map<K, V, H, E, TA, IA> &origin)
        {
            *this = origin;
        }

        /// ムーブします。
        /// @param origin ムーブ元です。
        Map(Map<K, V, H, E, TA, IA> &&origin) noexcept
        {
            *this = std::move(origin);
        }

        /// キーでアクセスします。
        /// @param key キーです。
        /// @exception KeyNotFoundException キーが含まれていない可能性があります。
        V &operator[](const K &key)
        {
            auto index = this->indexOf(key);
            if (index != USIZE_MAX)
            {
                return this->m_set.m_elements[this->m_set.m_indices[index]].key;
            }
            else
            {
                throw KeyNotFoundException(TXT("キーが含まれていませんでした。"));
            }
        }

        /// キーで取得します。
        /// @param key キーです。
        /// @exception KeyNotFoundException キーが含まれていない可能性があります。
        V operator[](const K &key) const
        {
            auto index = this->indexOf(key);
            if (index != USIZE_MAX)
            {
                return this->m_set.m_elements[this->m_set.m_indices[index]].key;
            }
            else
            {
                throw KeyNotFoundException(TXT("キーが含まれていませんでした。"));
            }
        }

        /// キーでアクセスします。
        /// @param key キーです。
        /// @exception KeyNotFoundException キーが含まれていない可能性があります。
        V &at(const K &key)
        {
            return (*this)[key];
        }

        /// キーで取得します。
        /// @param key キーです。
        /// @exception KeyNotFoundException キーが含まれていない可能性があります。
        V at(const K &key) const
        {
            return (*this)[key];
        }

        /// 要素を追加します。
        /// @param key 追加するキーです。
        /// @param value 追加する値です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void insert(const K &key, const V &value)
        {
            this->m_set.insert(Pair<K, V>(key, value));
        }

        /// 要素を追加します。
        /// @param key 追加するキーです。
        /// @param value 追加する値です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void insert(const K &key, const V &value)
        {
            this->m_set.insert(Pair<K, V>(key, value));
        }

        /// 要素を削除します。
        /// @param key 削除する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void remove(const K &key)
        {
            auto index = this->indexOf(key);
            if (index != USIZE_MAX)
            {
                this->m_set.removeByPositionIndex(index);
            }
        }

        /// 要素を削除します。
        /// @param key 削除する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void remove(K &&key)
        {
            auto index = this->indexOf(key);
            if (index != USIZE_MAX)
            {
                this->m_set.removeByPositionIndex(index);
            }
        }

        /// 要素が含まれているか判定します。
        /// @param key 判定するキーです。
        Bool contains(const K &key) const noexcept
        {
            return this->indexOf(key) != USIZE_MAX;
        }

        /// 要素が含まれているか判定します。
        /// @param key 判定するキーです。
        Bool contains(K &&key) const noexcept
        {
            return this->indexOf(key) != USIZE_MAX;
        }

        /// 要素数を取得します。
        /// @return 要素数です。
        USize count() const noexcept
        {
            return this->m_set.count();
        }

        /// 先頭の可変イテレータを取得します。
        /// @return 先頭イテレータです。
        PointerIterator<Pair<K, V>> begin() noexcept
        {
            return this->m_set.begin();
        }

        /// 先頭の不変イテレータを取得します。
        /// @return 先頭イテレータです。
        ConstPointerIterator<Pair<K, V>> begin() const noexcept
        {
            return this->m_set.begin();
        }

        /// 番兵の可変イテレータを取得します。
        /// @return 番兵イテレータです。
        PointerIterator<Pair<K, V>> end() noexcept
        {
            return this->m_set.end();
        }

        /// 番兵の不変イテレータを取得します。
        /// @return 番兵イテレータです。
        ConstPointerIterator<Pair<K, V>> end() const noexcept
        {
            return this->m_set.end();
        }
    };
}
#endif // !_FURAIENGINE_MAP_HPP