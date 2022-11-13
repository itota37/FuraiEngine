/// @file FuraiEngine/Allocation.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// メモリ管理機能を提供します。
#ifndef _FURAIENGINE_ALLOCATION_HPP
#define _FURAIENGINE_ALLOCATION_HPP

#include "FuraiEngine/Primitive.hpp"
#include "FuraiEngine/Exception.hpp"

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    /// メモリに関する例外です。
    DYNAMIC_LINK class MemoryException: public IException
    {
    public:
        
        /// この例外で挿入可能なメッセージ文字列の文字数上限です。
        static constexpr USize MESSAGE_LENGTH = 256;
    
    private:
    
        USize m_length;                  // エラーメッセージの長さです。
        Char m_message[MESSAGE_LENGTH]; // エラーメッセージです。
    
    public:

        /// 初期化します。
        /// @tparam N messageの文字数です。
        /// @param message エラーメッセージです。
        template<USize N>
        MemoryException(const Char (&message)[N]) noexcept
        : m_length(N)
        {
            static_assert(N <= MESSAGE_LENGTH, "This message is out of range.");

            for (USize i = 0; i < N; i++)
            {
                this->m_message[i] = message[i];
            }
        }

        /// コピーします。
        /// @param origine コピー元です。
        MemoryException(const MemoryException &origin) noexcept;

        /// ムーブします。
        /// @param origine ムーブ元です。
        MemoryException(MemoryException &&origin) noexcept;

        /// コピー代入します。
        /// @param origine コピー元です。
        MemoryException& operator=(const MemoryException &origin) noexcept;

        /// ムーブ代入します。
        /// @param origine ムーブ元です。
        MemoryException& operator=(MemoryException &&origin) noexcept;

        /// エラーメッセージを取得します。
        /// @return エラーメッセージです。
        const Char *message() const noexcept override;
    };

    /// 固定長メモリプールです。
    DYNAMIC_LINK class MemoryPool
    {
        USize m_elementSize;       // 1要素のサイズです。sizeof(U8*)以上に矯正されます。
        USize m_elementCount;      // このプールが管理する要素の数です。1以上に矯正されます。
        U8 *m_bufferPointer;       // メモリバッファです。
        USize m_bufferAddressMin;  // メモリバッファのアドレスの最小値です。
        USize m_bufferAddressMax;  // メモリバッファのアドレスの最大値です。
        U8 **m_freeElementListTop; // 現在使用されていない要素のリストの先頭へのポインタです。
        USize m_freeElementCount;  // 現在使用されていない要素の数です。0でプールが空、m_elementCountと同等で1つも使用されていない状態です。
    
    public:
        
        /// 初期化します。
        /// @param size 1要素のサイズです。 @n sizeof(U8*)以上に矯正されます。
        /// @param count このプールで管理する要素の数です。 @n 1以上に矯正されます。
        /// @exception MemoryException メモリ確保に失敗する可能性があります。
        MemoryPool(USize size, USize count);

        /// コピー不可です。
        MemoryPool(const MemoryPool &) = delete;

        
        /// ムーブ不可です。
        MemoryPool(MemoryPool &&) = delete;

        
        /// コピー代入不可です。
        MemoryPool& operator=(const MemoryPool &) = delete;

        
        /// ムーブ代入不可です。
        MemoryPool& operator=(MemoryPool &&) = delete;

        /// 解体します。
        ~MemoryPool() noexcept;

        /// メモリ要素を確保します。
        /// @return 確保したメモリ要素のポインタです。
        /// @retval nullptr このプールから確保可能なメモリ要素がありませんでした。
        void *allocate() noexcept;

        /// メモリ要素を解放します。
        /// @param pointer 解放するメモリ要素のポインタです。
        void deallocate(void *pointer) noexcept;

        /// このプールが管理するメモリアドレスの最小値を取得します。
        /// @return このプールが管理するメモリアドレスの最小値です。
        USize managedAddresMin() const noexcept;
        
        /// このプールが管理するメモリアドレスの最大値を取得します。
        /// @return このプールが管理するメモリアドレスの最大値です。
        USize managedAddresMax() const noexcept;

        /// このプールが管理するメモリ要素の数を取得します。
        /// @return このプールが管理するメモリ要素の数です。
        USize memoryElementCount() const noexcept;

        /// 現在確保可能なメモリ要素の数を取得します。
        /// @return 現在確保可能なメモリ要素の数です。 @n 0の場合確保可能な要素はありません。 @n memoryElementCountと等しい場合は現在使用中の要素はありません。
        USize freeMemoryElementCount() const noexcept;
    };

    /// 固定長メモリマネージャです。
    DYNAMIC_LINK class FixedMemoryManager
    {
        USize m_elementSize;     // 1要素のサイズです。sizeof(U8*)以上に矯正されます。
        USize m_elementCount;    // 1プールが管理する要素の数です。1以上に矯正されます。
        USize m_poolCount;       // このマネージャが管理するメモリプールの数です。
        MemoryPool **m_poolList; // このマネージャが管理するメモリプールのリストです。
        MemoryPool *m_allocPool; // メモリ確保対象のメモリプールです。
    
    public:

        /// 初期化します。
        /// @param size 1要素のサイズです。 @n sizeof(U8*)以上に矯正されます。
        /// @param count 1つのプールで管理する要素の数です。 @n 1以上に矯正されます。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        FixedMemoryManager(USize size, USize count);

        /// コピー不可です。
        FixedMemoryManager(const FixedMemoryManager &) = delete;

        
        /// ムーブ不可です。
        FixedMemoryManager(FixedMemoryManager &&) = delete;

        
        /// コピー代入不可です。
        FixedMemoryManager& operator=(const FixedMemoryManager &) = delete;

        
        /// ムーブ代入不可です。
        FixedMemoryManager& operator=(FixedMemoryManager &&) = delete;

        /// 解体します。
        ~FixedMemoryManager() noexcept;

        /// メモリを確保します。
        /// @return 確保したメモリのポインタです。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        void *allocate();

        /// メモリを解放します。
        /// @param pointer 解放するメモリのポインタです。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        void deallocate(void *pointer);

        /// 1つのプールが管理する要素の数を取得します。
        /// 1以上に矯正されます。
        /// @return 1つのプールが管理する要素の数です。
        USize poolManagedElementCount() const noexcept;
        
        /// 1つのプールが管理する要素の数を設定します。
        /// 1以上に矯正されます。
        /// @param count 1つのプールが管理する要素の数です。
        void poolManagedElementCount(USize count) noexcept;
    };

    /// 可変長メモリマネージャです。
    DYNAMIC_LINK class DynamicMemoryManager
    {
        static constexpr USize SIZE16 = 16;
        static constexpr USize SIZE32 = 32;
        static constexpr USize SIZE64 = 64;
        static constexpr USize SIZE128 = 128;
        static constexpr USize SIZE256 = 256;

        FixedMemoryManager m_memory16;  // 16バイト以下のメモリマネージャです。
        FixedMemoryManager m_memory32;  // 32バイト以下のメモリマネージャです。
        FixedMemoryManager m_memory64;  // 64バイト以下のメモリマネージャです。
        FixedMemoryManager m_memory128;  // 128バイト以下のメモリマネージャです。
        FixedMemoryManager m_memory256;  // 256バイト以下のメモリマネージャです。

    public:

        /// 初期化します。
        /// @param memory16Count 16バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
        /// @param memory32Count 32バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
        /// @param memory64Count 64バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
        /// @param memory128Count 128バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
        /// @param memory256Count 256バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        DynamicMemoryManager(USize memory16Count, USize memory32Count, USize memory64Count, USize memory128Count, USize memory256Count);

        /// コピー不可です。
        DynamicMemoryManager(const DynamicMemoryManager &) = delete;

        
        /// ムーブ不可です。
        DynamicMemoryManager(DynamicMemoryManager &&) = delete;

        
        /// コピー代入不可です。
        DynamicMemoryManager& operator=(const DynamicMemoryManager &) = delete;

        
        /// ムーブ代入不可です。
        DynamicMemoryManager& operator=(DynamicMemoryManager &&) = delete;

        /// メモリを確保します。
        /// @param size 確保するメモリのサイズです。
        /// @return 確保したメモリのポインタです。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        void *allocate(USize size);

        /// メモリを解放します。
        /// @param pointer 解放するメモリのポインタです。
        /// @param size 解放するメモリのサイズです。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        void deallocate(void *pointer, USize size);

        /// 16バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 16バイト以下のメモリを管理するプールの要素数です。
        USize pool16ElementCount() const noexcept;

        /// 16バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 16バイト以下のメモリを管理するプールの要素数です。
        void pool16ElementCount(USize count) noexcept;
    
        /// 32バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 32バイト以下のメモリを管理するプールの要素数です。
        USize pool32ElementCount() const noexcept;

        /// 32バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 32バイト以下のメモリを管理するプールの要素数です。
        void pool32ElementCount(USize count) noexcept;
        
        /// 64バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 64バイト以下のメモリを管理するプールの要素数です。
        USize pool64ElementCount() const noexcept;

        /// 64バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 64バイト以下のメモリを管理するプールの要素数です。
        void pool64ElementCount(USize count) noexcept;
        
        /// 128バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 128バイト以下のメモリを管理するプールの要素数です。
        USize pool128ElementCount() const noexcept;

        /// 128バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 128バイト以下のメモリを管理するプールの要素数です。
        void pool128ElementCount(USize count) noexcept;
        
        /// 256バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 256バイト以下のメモリを管理するプールの要素数です。
        USize pool256ElementCount() const noexcept;

        /// 256バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 256バイト以下のメモリを管理するプールの要素数です。
        void pool256ElementCount(USize count) noexcept;
    };

    /// グローバルメモリメモリマネージャです。
    DYNAMIC_LINK class GlobalMemoryManager
    {
    public:

        /// コピー不可です。
        GlobalMemoryManager(const GlobalMemoryManager &) = delete;

        
        /// ムーブ不可です。
        GlobalMemoryManager(GlobalMemoryManager &&) = delete;

        
        /// コピー代入不可です。
        GlobalMemoryManager& operator=(const GlobalMemoryManager &) = delete;

        
        /// ムーブ代入不可です。
        GlobalMemoryManager& operator=(GlobalMemoryManager &&) = delete;

        /// メモリを確保します。
        /// @param size 確保するメモリのサイズです。
        /// @return 確保したメモリのポインタです。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        static void *allocate(USize size);

        /// メモリを解放します。
        /// @param pointer 解放するメモリのポインタです。
        /// @param size 解放するメモリのサイズです。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        static void deallocate(void *pointer, USize size);

        /// 16バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 16バイト以下のメモリを管理するプールの要素数です。
        static USize pool16ElementCount() noexcept;

        /// 16バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 16バイト以下のメモリを管理するプールの要素数です。
        static void pool16ElementCount(USize count) noexcept;
    
        /// 32バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 32バイト以下のメモリを管理するプールの要素数です。
        static USize pool32ElementCount() noexcept;

        /// 32バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 32バイト以下のメモリを管理するプールの要素数です。
        static void pool32ElementCount(USize count) noexcept;
        
        /// 64バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 64バイト以下のメモリを管理するプールの要素数です。
        static USize pool64ElementCount() noexcept;

        /// 64バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 64バイト以下のメモリを管理するプールの要素数です。
        static void pool64ElementCount(USize count) noexcept;
        
        /// 128バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 128バイト以下のメモリを管理するプールの要素数です。
        static USize pool128ElementCount() noexcept;

        /// 128バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 128バイト以下のメモリを管理するプールの要素数です。
        static void pool128ElementCount(USize count) noexcept;
        
        /// 256バイト以下のメモリを管理するプールの要素数を取得します。
        /// @return 256バイト以下のメモリを管理するプールの要素数です。
        static USize pool256ElementCount() noexcept;

        /// 256バイト以下のメモリを管理するプールの要素数を設定します。
        /// @param count 256バイト以下のメモリを管理するプールの要素数です。
        static void pool256ElementCount(USize count) noexcept;
    };

    /// 標準アロケータです。
    /// @tparam T 確保するメモリ上に構築する型です。
    template<typename T>
    class Allocator
    {
    public:

        /// 確保するメモリ上に構築する型の型エイリアスです。
        using value_type = T;

        /// コンテナのムーブ時にアロケータを伝搬させるかのフラグ型です。
        using propagate_on_container_move_assignment = std::true_type;

        /// 同型のアロケータでは、アロケータ間を超えて確保、解放できるかどうかのフラグ型です。
        using is_always_equal = std::true_type;

        /// 初期化します。
        Allocator() noexcept
        {}

        /// コピーします。
        /// @tparam U コピー元の要素の型です。
        template<typename U>
        Allocator(const Allocator<U> &) noexcept
        {}

        /// ムーブします。
        /// @tparam U ムーブ元の要素の型です。
        template<typename U>
        Allocator(const Allocator<U> &) noexcept
        {}

        /// コピー代入します。
        /// @tparam U コピー元の要素の型です。
        template<typename U>
        Allocator& operator=(const Allocator<U> &) noexcept
        {}

        /// ムーブ代入します。
        /// @tparam U ムーブ元の要素の型です。
        template<typename U>
        Allocator& operator=(const Allocator<U> &) noexcept
        {}

        /// メモリを確保します。
        /// @param size 確保するメモリのサイズです。
        /// @return 確保したメモリのポインタです。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        T *allocate(USize size)
        {
            return reinterpret_cast<T*>(GlobalMemoryManager::allocate(size));
        }

        /// メモリを解放します。
        /// @param pointer 解放するメモリのポインタです。
        /// @param size 解放するメモリのサイズです。
        /// @exception MemoryException メモリプールの確保に失敗する可能性があります。
        void deallocate(T *pointer, USize size)
        {
            GlobalMemoryManager::deallocate(reinterpret_cast<void*>(pointer), size);
        }
    };

    /// 一方のアロケータで確保したメモリをもう一方で解放可能か判定します。
    /// @tparam T 左辺引数の要素の型です。
    /// @tparam U 右辺引数の要素の型です。
    template<typename T, typename U>
    constexpr Bool operator==(const Allocator<T>&, const Allocator<U>&) noexcept
    {
        return true;
    }

    /// 一方のアロケータで確保したメモリをもう一方で解放不可能か判定します。
    /// @tparam T 左辺引数の要素の型です。
    /// @tparam U 右辺引数の要素の型です。
    template<typename T, typename U>
    constexpr Bool operator!=(const Allocator<T>&, const Allocator<U>&) noexcept
    {
        return false;
    }
}

#endif // !_FURAIENGINE_ALLOCATION_HPP