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
    class MemoryException: public IException
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
        const Char *message() noexcept override;
    };

    /// 固定長メモリプールです。
    class MemoryPool
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

        /// 解体します。
        ~MemoryPool() noexcept;
    };

    /// グローバルメモリからメモリを確保します。
    /// @param size 確保するメモリサイズです。
    /// @return 確保したメモリのポインタです。
    void *allocate(USize size);

    /// グローバルメモリのメモリを解放します。
    /// @param pointer 確保したメモリのポインタです。
    /// @param size 確保したメモリのサイズです。
    void deallocate(void *pointer, USize size);
}

/// new演算子の定義です。
/// @param size 確保するメモリサイズです。
/// @return 確保したメモリのポインタです。
void *operator new(FuraiEngine::USize size);

/// delete演算子の定義です。
/// @param pointer 確保したメモリのポインタです。
/// @param size 確保したメモリのサイズです。
void operator delete(void *pointer, FuraiEngine::USize size);

#endif // !_FURAIENGINE_ALLOCATION_HPP