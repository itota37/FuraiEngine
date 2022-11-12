/// @file FuraiEngine/Allocation.cpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// Allocation.hppの定義です。

#include <cstdlib>
#include "FuraiEngine/Allocation.hpp"

using namespace FuraiEngine;

// --------------------
//
// MemoryException
//
// ====================

/// コピーします。
/// @param origine コピー元です。
MemoryException::MemoryException(const MemoryException &origin) noexcept
: m_length(origin.m_length)
{
    for (USize i = 0; i < this->m_length; i++)
    {
        this->m_message[i] = origin.m_message[i];
    }
}

/// ムーブします。
/// @param origine ムーブ元です。
MemoryException::MemoryException(MemoryException &&origin) noexcept
: m_length(origin.m_length)
{
    for (USize i = 0; i < this->m_length; i++)
    {
        this->m_message[i] = origin.m_message[i];
    }
}

/// コピー代入します。
/// @param origine コピー元です。
MemoryException& MemoryException::operator=(const MemoryException &origin) noexcept
{
    this->m_length = origin.m_length;
    for (USize i = 0; i < this->m_length; i++)
    {
        this->m_message[i] = origin.m_message[i];
    }
    return *this;
}

/// ムーブ代入します。
/// @param origine ムーブ元です。
MemoryException& MemoryException::operator=(MemoryException &&origin) noexcept
{
    this->m_length = origin.m_length;
    for (USize i = 0; i < this->m_length; i++)
    {
        this->m_message[i] = origin.m_message[i];
    }
    return *this;
}

/// エラーメッセージを取得します。
/// @return エラーメッセージです。
const Char *MemoryException::message() noexcept
{
    return this->m_message;
}

// --------------------
//
// MemoryPool
//
// ====================

/// 初期化します。
/// @param size 1要素のサイズです。 @n sizeof(U8*)以上に矯正されます。
/// @param count このプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @exception MemoryException メモリ確保に失敗する可能性があります。
MemoryPool::MemoryPool(USize size, USize count)
: m_elementSize((size < sizeof(U8*) ? sizeof(U8*) : size))
, m_elementCount((count < 1 ? 1 : count))
, m_bufferPointer(reinterpret_cast<U8*>(std::malloc(m_elementSize * m_elementCount)))
, m_freeElementListTop(nullptr)
, m_freeElementCount(m_elementCount)
{
    if (!this->m_bufferPointer) throw MemoryException(TXT("メモリを確保できませんでした。"));

    auto addrTop = reinterpret_cast<USize>(&this->m_bufferPointer[0]);
    auto addrEnd = reinterpret_cast<USize>(&this->m_bufferPointer[m_elementSize * m_elementCount - 1]);
    if (addrTop < addrEnd)
    {
        this->m_bufferAddressMin = addrTop;
        this->m_bufferAddressMax = addrEnd;
    }
    else
    {
        this->m_bufferAddressMin = addrEnd;
        this->m_bufferAddressMax = addrTop;
    }
}
/// 解体します。
MemoryPool::~MemoryPool()
{
    if (this->m_bufferPointer)
    {
        std::free(this->m_bufferPointer);
    }
}

// GlobalMemoryControl *globalMemoryControl;

#ifdef _FURAIENGINE_CORE_DYANMIC
// std::once_flag initGlobalMemoryControlFlag;
// void initGlobalMemoryControl()
// {
//     
// }
#endif

/// グローバルメモリからメモリを確保します。
/// @param size 確保するメモリサイズです。
/// @return 確保したメモリのポインタです。
void *allocate(USize size)
{

}

/// グローバルメモリのメモリを解放します。
/// @param pointer 確保したメモリのポインタです。
/// @param size 確保したメモリのサイズです。
void deallocate(void *pointer, USize size)
{

}

/// new演算子の定義です。
/// @param size 確保するメモリサイズです。
/// @return 確保したメモリのポインタです。
void *operator new(FuraiEngine::USize size)
{
    return FuraiEngine::allocate(size);
}

/// delete演算子の定義です。
/// @param pointer 確保したメモリのポインタです。
/// @param size 確保したメモリのサイズです。
void operator delete(void *pointer, FuraiEngine::USize size)
{
    return FuraiEngine::deallocate(pointer, size);
}