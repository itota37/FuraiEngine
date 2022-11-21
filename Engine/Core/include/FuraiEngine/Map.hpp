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
    /// キーと値のペア構造です。
    /// @tparam K キーの型です。
    /// @tparam V 値の型です。
    template<typename K, typename V>
    struct Pair
    {
        /// キーです。
        K key;
        /// 値です。
        V value;

        /// 初期化します。
        Pair(const K &key, const V &value)
        : key(key)
        , value(value)
        {}

        /// コピー代入します。
        /// @param origin コピー元です。
        Pair<K, V>& operator=(const Pair<K, V> &origin)
        {
            this->key = origin.key;
            this->value = origin.value;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        Pair<K, V>& operator=(Pair<K, V> &&origin) noexcept
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
    /// ハッシュ関数オブジェクトのPair特殊化構造体です。
    /// @tparam K キーの型です。
    /// @tparam V 値の型です。
    template<typename K, typename V>
    struct HashOf<Pair<K, V>>
    {
        /// ハッシュ値を取得します。
        /// @param value ハッシュ値を計算する値です。
        /// @return ハッシュ値です。
        USize operator()(const Pair<K, V> &value) const noexcept
        {
            return HashOf<K>{}(value.key);
        }
    };
    /// @endcond

    /// @cond FURAIDOC_INTERNAL
    /// 等価関数オブジェクトのPair特殊化構造体です。
    /// @tparam K キーの型です。
    /// @tparam V 値の型です。
    template<typename K, typename V>
    struct EqualOf<Pair<K, V>>
    {
        /// 等しいか判定します。
        /// @param l 比較する左辺値です。
        /// @param r 比較する右辺値です。
        /// @return 等しい場合は、真を返します。
        Bool operator()(const Pair<K, V> &l, const Pair<K, V> &r) const noexcept
        {
            return l.key == r.key;
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
    template<typename K, typename V, typename H = HashOf<Pair<K, V>>, typename E = EqualOf<Pair<K, V>>, typename PA = Allocator<Pair<K, V>>, typename IA = Allocator<USize>>
    class Map
    {
        Set<Pair<K, V>, H, E, PA, IA> m_set;

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
            const IA &indicesAllocator = IA()
        )
        : m_set(length, hashOf, equalOf, elementsAllocator, indicesAllocator)
        {}

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
            const IA &indicesAllocator = IA()
        )
        : m_set(0, hashOf, equalOf, elementsAllocator, indicesAllocator)
        {}

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
            const IA &indicesAllocator = IA()
        )
        : m_set(list, hashOf, equalOf, elementsAllocator, indicesAllocator)
        {}

        /// コピー代入します。
        /// @param origin コピー代入元です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        Map<K, V, H, E, PA, IA> &operator=(const Map<K, V, H, E, PA, IA> &origin)
        {
            this->m_set = origin.m_set;
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

        }

        /// キーで取得します。
        /// @param key キーです。
        /// @exception KeyNotFoundException キーが含まれていない可能性があります。
        V operator[](const K &key) const
        {
            
        }

        /// キーでアクセスします。
        /// @param key キーです。
        /// @exception KeyNotFoundException キーが含まれていない可能性があります。
        V &at(const K &key)
        {

        }

        /// キーで取得します。
        /// @param key キーです。
        /// @exception KeyNotFoundException キーが含まれていない可能性があります。
        V at(const K &key) const
        {
            
        }

        /// 要素を追加します。
        /// @param value 追加する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void add(const Pair<K, V> &value)
        {
            this->m_set.add(value);
        }

        /// 要素を追加します。
        /// @param value 追加する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void add(Pair<K, V> &&value)
        {
            this->m_set.add(value);
        }

        /// 要素を削除します。
        /// @param key 削除する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void remove(const K &key)
        {
        }

        /// 要素を削除します。
        /// @param key 削除する要素です。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        void remove(K &&key)
        {
        }

        /// 要素が含まれているか判定します。
        /// @param key 判定するキーです。
        Bool contains(const K &key) const noexcept 
        {
        }

        /// 要素が含まれているか判定します。
        /// @param key 判定するキーです。
        Bool contains(K &&key) const noexcept 
        {
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