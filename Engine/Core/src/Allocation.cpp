/// @file FuraiEngine/Allocation.cpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// Allocation.hppの定義です。

#include <new>
#include <mutex>
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
const Char *MemoryException::message() const noexcept
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
    
    // バッファの範囲を取得します。
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

    // 要素のリストを作成します。
    //
    // m_bufferPointer [elem][elem][elem]...
    //                 |  ^  |  ^  |  ^
    //      nullptr <--'  '--'  '--'  '-- m_freeElementListTop
    //
    auto step = this->m_elementSize;
    auto length = step * this->m_elementCount;
    for (USize i = 0; i < length; i += step)
    {
        auto ptr = reinterpret_cast<U8**>(&this->m_bufferPointer[i]);
        *ptr = reinterpret_cast<U8*>(this->m_freeElementListTop);
        this->m_freeElementListTop = ptr;
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

/// メモリ要素を確保します。
/// @return 確保したメモリ要素のポインタです。
/// @retval nullptr このプールから確保可能なメモリ要素がありませんでした。
void *MemoryPool::allocate() noexcept
{
    auto ptr = this->m_freeElementListTop;
    if (ptr)
    {
        this->m_freeElementCount -= 1;
        this->m_freeElementListTop = reinterpret_cast<U8**>(*ptr);
    }
    return reinterpret_cast<void*>(ptr);
}

/// メモリ要素を解放します。
/// @param pointer 解放するメモリ要素のポインタです。
void MemoryPool::deallocate(void *pointer) noexcept
{
    auto ptr = reinterpret_cast<U8**>(pointer);
    auto addr = reinterpret_cast<USize>(pointer);
    if (
        ptr 
        && this->m_bufferAddressMin <= addr
        && addr <= this->m_bufferAddressMax
    )
    {
        this->m_freeElementCount += 1;
        *ptr = reinterpret_cast<U8*>(this->m_freeElementListTop);
        this->m_freeElementListTop = ptr;
    }
}


/// このプールが管理するメモリアドレスの最小値を取得します。
/// @return このプールが管理するメモリアドレスの最小値です。
USize MemoryPool::managedAddresMin() const noexcept
{
    return this->m_bufferAddressMin;
}
        
/// このプールが管理するメモリアドレスの最大値を取得します。
/// @return このプールが管理するメモリアドレスの最大値です。
USize MemoryPool::managedAddresMax() const noexcept
{
    return this->m_bufferAddressMax;
}

/// このプールが管理するメモリ要素の数を取得します。
/// @return このプールが管理するメモリ要素の数です。
USize MemoryPool::memoryElementCount() const noexcept
{
    return this->m_elementCount;
}

/// 現在確保可能なメモリ要素の数を取得します。
/// @return 現在確保可能なメモリ要素の数です。 @n 0の場合確保可能な要素はありません。 @n memoryElementCountと等しい場合は現在使用中の要素はありません。
USize MemoryPool::freeMemoryElementCount() const noexcept
{
    return this->m_freeElementCount;
}

// --------------------
//
// FixedMemoryManager
//
// ====================

/// メモリプールをヒープ上に生成します。
/// @param size 1要素のサイズです。 @n sizeof(U8*)以上に矯正されます。
/// @param count このプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @return 生成したメモリプールのポインタです。
/// @exception MemoryException メモリ確保に失敗する可能性があります。
MemoryPool *newMemoryPool(USize size, USize count)
{
    auto poolSize = sizeof(MemoryPool);
    auto poolPtr = std::malloc(poolSize);
    return new(poolPtr) MemoryPool(size, count);
}

/// ヒープ上のメモリプールをを削除します。
/// @param pointer 削除するメモリプールのポインタです。
void deleteMemoryPool(MemoryPool *pointer) noexcept
{
    if (pointer)
    {
        pointer->~MemoryPool();
        std::free(pointer);
    }
}

/// メモリプールリストをヒープ上に作成します。
/// @param count 要素数です。
/// @return 作成したメモリプールリストのポインタです。
/// @exception MemoryException メモリ確保に失敗する可能性があります。
MemoryPool **newMemoryPoolList(USize count) noexcept
{
    auto listSize = sizeof(MemoryPool*) * count;
    auto listPtr = std::malloc(listSize);
    return new(listPtr) MemoryPool*();
}

/// メモリプールリストをヒープ上から削除します。
/// @param pointer 削除するメモリプールリストのポインタです。
void deleteMemoryPoolList(MemoryPool **pointer) noexcept
{
    if (pointer)
    {
        std::free(pointer);
    }
}

/// 初期メモリプールリストを作成します。
/// @param size 1要素のサイズです。 @n sizeof(U8*)以上に矯正されます。
/// @param count 1つのプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @param length 要素数です。
/// @return 作成したメモリプールリストのポインタです。
/// @exception MemoryException メモリ確保に失敗する可能性があります。
MemoryPool **initMemoryPoolList(USize size, USize count, USize length)
{
    auto list = newMemoryPoolList(length);
    for (USize i = 0; i < length; i++)
    {
        list[i] = newMemoryPool(size, count);
    }
    return list;
}

/// メモリプールリストを解体します。
/// @param count 解体するメモリプールリストの要素数です。
/// @param pointer 解体するメモリプールリストのポインタです。
void finMemoryPoolList(USize count, MemoryPool **pointer) noexcept
{
    if (pointer)
    {
        for (USize i = 0; i < count; i++)
        {
            deleteMemoryPool(pointer[i]);
        }
        deleteMemoryPoolList(pointer);
    }
}

/// メモリプールをメモリプールリストに追加します。
/// @param size 1要素のサイズです。 @n sizeof(U8*)以上に矯正されます。
/// @param count 1つのプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @param length メモリプールリストの要素数の参照です。 @n 追加分加算されます。
/// @param list メモリプールリストの参照です。 @n 追加されたメモリプールリストに上書きされます。
/// @return 追加されたメモリプールのポインタです。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
MemoryPool *addMemoryPool(USize size, USize count, USize &length, MemoryPool **&list)
{
    // 新規作成します。
    auto pool = newMemoryPool(size, count);
    auto poolListCount = length + 1;
    auto poolList = newMemoryPoolList(poolListCount);
    // 要素を移動させます。
    USize i = 0;
    while (i < length)
    {
        if (list[i]->managedAddresMin() < pool->managedAddresMin())
        {
            poolList[i] = list[i];
            i += 1;
        }
        else
        {
            break;
        }
    }
    USize j = i;
    poolList[i] = pool;
    i += 1;
    while (j < length)
    {
        poolList[i] = list[j];
        i += 1;
        j += 1;
    }
    // 古いデータを削除します。
    deleteMemoryPoolList(list);
    // 新しいデータで上書きします。
    length = poolListCount;
    list = poolList;

    return pool;
}

/// メモリプールリストからメモリを削除します。
/// @param index 削除するメモリプールの位置です。
/// @param length メモリプールリストの要素数の参照です。 @n 追加分加算されます。
/// @param list メモリプールリストの参照です。 @n 追加されたメモリプールリストに上書きされます。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
void removeMemoryPool(USize index, USize &length, MemoryPool **&list)
{
    // 新規作成します。
    auto poolListCount = length - 1;
    auto poolList = newMemoryPoolList(poolListCount);
    // 要素を移動させます。
    USize i = 0;
    for (; i < index; i++)
    {
        poolList[i] = list[i];
    }
    USize j = i;
    j += 1;
    for (; j < length; i++, j++)
    {
        poolList[i] = list[j];
    }
    // 古いデータを削除します。
    deleteMemoryPool(list[index]);
    deleteMemoryPoolList(list);
    // 新しいデータで上書きします。
    length = poolListCount;
    list = poolList;

}

/// 初期化します。
/// @param size 1要素のサイズです。 @n sizeof(U8*)以上に矯正されます。
/// @param count 1つのプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
FixedMemoryManager::FixedMemoryManager(USize size, USize count)
: m_elementSize(size)
, m_elementCount(count)
, m_poolCount(1)
, m_poolList(initMemoryPoolList(m_elementSize, m_elementCount, m_poolCount))
, m_allocPool(m_poolList[0])
{}

/// 解体します。
FixedMemoryManager::~FixedMemoryManager() noexcept
{
    finMemoryPoolList(this->m_poolCount, this->m_poolList);
}

/// メモリを確保します。
/// @return 確保したメモリのポインタです。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
void *FixedMemoryManager::allocate()
{
    // メモリプールが空の場合、メモリプールを確保します。
    if (this->m_allocPool->freeMemoryElementCount() == 0)
    {
        this->m_allocPool = addMemoryPool(
            this->m_elementSize, 
            this->m_elementCount, 
            this->m_poolCount, 
            this->m_poolList
        );
    }
    // メモリを確保します。
    this->m_allocPool->allocate();
}

/// メモリを解放します。
/// @param pointer 解放するメモリのポインタです。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
void FixedMemoryManager::deallocate(void *pointer)
{
    // アドレスから管理プールを走査します。
    auto addr = reinterpret_cast<USize>(pointer);
    auto l = 0;
    auto r = this->m_poolCount;
    auto p = static_cast<USize>(r * 0.5f);
    while (1 < r - l)
    {
        if (this->m_poolList[p]->managedAddresMin() <= addr)
        {
            l = p;
        }
        else if (addr < this->m_poolList[p]->managedAddresMin())
        {
            r = p;
        }
        p = l + static_cast<USize>(l - r * 0.5f);
    }
    // メモリを解放します。
    this->m_poolList[p]->deallocate(pointer);
    // メモリプールの使用中の要素がない場合、メモリプールを解放します。
    if (this->m_poolList[p]->memoryElementCount() == this->m_poolList[p]->freeMemoryElementCount())
    {
        removeMemoryPool(p, this->m_poolCount, this->m_poolList);
    }
}

/// 1つのプールが管理する要素の数を取得します。
/// 1以上に矯正されます。
/// @return 1つのプールが管理する要素の数です。
USize FixedMemoryManager::poolManagedElementCount() const noexcept
{
    return this->m_elementCount;
}
        
/// 1つのプールが管理する要素の数を設定します。
/// 1以上に矯正されます。
/// @param count 1つのプールが管理する要素の数です。
void FixedMemoryManager::poolManagedElementCount(USize count) noexcept
{
    this->m_elementCount = count < 1 ? 1 : count;
}

// --------------------
//
// DynamicMemoryManager
//
// ====================

/// 初期化します。
/// @param memory16Count 16バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @param memory32Count 32バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @param memory64Count 64バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @param memory128Count 128バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @param memory256Count 256バイトメモリプールで管理する要素の数です。 @n 1以上に矯正されます。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
DynamicMemoryManager::DynamicMemoryManager(USize memory16Count, USize memory32Count, USize memory64Count, USize memory128Count, USize memory256Count)
: m_memory16(SIZE16, memory16Count)
, m_memory32(SIZE32, memory32Count)
, m_memory64(SIZE64, memory64Count)
, m_memory128(SIZE128, memory128Count)
, m_memory256(SIZE256, memory256Count)
{}
        

/// メモリを確保します。
/// @param size 確保するメモリのサイズです。
/// @return 確保したメモリのポインタです。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
void *DynamicMemoryManager::allocate(USize size)
{
    if (size <= SIZE16)
    {
        return this->m_memory16.allocate();
    }
    else if (size <= SIZE32)
    {
        return this->m_memory32.allocate();
    }
    else if (size <= SIZE64)
    {
        return this->m_memory64.allocate();
    }
    else if (size <= SIZE128)
    {
        return this->m_memory128.allocate();
    }
    else if (size <= SIZE256)
    {
        return this->m_memory256.allocate();
    }
    else
    {
        return std::malloc(size);
    }
}

/// メモリを解放します。
/// @param pointer 解放するメモリのポインタです。
/// @param size 解放するメモリのサイズです。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
void DynamicMemoryManager::deallocate(void *pointer, USize size)
{
    if (size <= SIZE16)
    {
        this->m_memory16.deallocate(pointer);
    }
    else if (size <= SIZE32)
    {
        this->m_memory32.deallocate(pointer);
    }
    else if (size <= SIZE64)
    {
        this->m_memory64.deallocate(pointer);
    }
    else if (size <= SIZE128)
    {
        this->m_memory128.deallocate(pointer);
    }
    else if (size <= SIZE256)
    {
        this->m_memory256.deallocate(pointer);
    }
    else
    {
        std::free(pointer);
    }
}

/// 16バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 16バイト以下のメモリを管理するプールの要素数です。
USize DynamicMemoryManager::pool16ElementCount() const noexcept
{
    return this->m_memory16.poolManagedElementCount();
}

/// 16バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 16バイト以下のメモリを管理するプールの要素数です。
void DynamicMemoryManager::pool16ElementCount(USize count) noexcept
{
    this->m_memory16.poolManagedElementCount(count);
}
    
/// 32バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 32バイト以下のメモリを管理するプールの要素数です。
USize DynamicMemoryManager::pool32ElementCount() const noexcept
{
    return this->m_memory32.poolManagedElementCount();
}

/// 32バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 32バイト以下のメモリを管理するプールの要素数です。
void DynamicMemoryManager::pool32ElementCount(USize count) noexcept
{
    this->m_memory32.poolManagedElementCount(count);
}
        
/// 64バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 64バイト以下のメモリを管理するプールの要素数です。
USize DynamicMemoryManager::pool64ElementCount() const noexcept
{
    return this->m_memory64.poolManagedElementCount();
}

/// 64バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 64バイト以下のメモリを管理するプールの要素数です。
void DynamicMemoryManager::pool64ElementCount(USize count) noexcept
{
    this->m_memory64.poolManagedElementCount(count);
}
        
/// 128バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 128バイト以下のメモリを管理するプールの要素数です。
USize DynamicMemoryManager::pool128ElementCount() const noexcept
{
    return this->m_memory128.poolManagedElementCount();
}

/// 128バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 128バイト以下のメモリを管理するプールの要素数です。
void DynamicMemoryManager::pool128ElementCount(USize count) noexcept
{
    this->m_memory128.poolManagedElementCount(count);
}
        
/// 256バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 256バイト以下のメモリを管理するプールの要素数です。
USize DynamicMemoryManager::pool256ElementCount() const noexcept
{
    return this->m_memory256.poolManagedElementCount();
}

/// 256バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 256バイト以下のメモリを管理するプールの要素数です。
void DynamicMemoryManager::pool256ElementCount(USize count) noexcept
{
    this->m_memory256.poolManagedElementCount(count);
}

// --------------------
//
// グローバルメモリ
//
// ====================

constexpr USize INIT_COUNT16 = 32;
constexpr USize INIT_COUNT32 = 32;
constexpr USize INIT_COUNT64 = 32;
constexpr USize INIT_COUNT128 = 16;
constexpr USize INIT_COUNT256 = 16;

DynamicMemoryManager *g_globalMemoryControl;
std::once_flag g_initGlobalMemoryFlag;
std::mutex g_globalMemoryControlMutex;
void initGlobalMemory()
{
    auto size = sizeof(DynamicMemoryManager);
    auto ptr = std::malloc(size);
    g_globalMemoryControl = new(ptr) DynamicMemoryManager(
        INIT_COUNT16, 
        INIT_COUNT32, 
        INIT_COUNT64, 
        INIT_COUNT128, 
        INIT_COUNT256
    );
}

/// メモリを確保します。
/// @param size 確保するメモリのサイズです。
/// @return 確保したメモリのポインタです。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
void *GlobalMemoryManager::allocate(USize size)
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    return g_globalMemoryControl->allocate(size);
}

/// メモリを解放します。
/// @param pointer 解放するメモリのポインタです。
/// @param size 解放するメモリのサイズです。
/// @exception MemoryException メモリプールの確保に失敗する可能性があります。
void GlobalMemoryManager::deallocate(void *pointer, USize size)
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    g_globalMemoryControl->deallocate(pointer, size);
}

/// 16バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 16バイト以下のメモリを管理するプールの要素数です。
USize GlobalMemoryManager::pool16ElementCount() noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    return g_globalMemoryControl->pool16ElementCount();
}

/// 16バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 16バイト以下のメモリを管理するプールの要素数です。
void GlobalMemoryManager::pool16ElementCount(USize count) noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    g_globalMemoryControl->pool16ElementCount(count);
}
    
/// 32バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 32バイト以下のメモリを管理するプールの要素数です。
USize GlobalMemoryManager::pool32ElementCount() noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    return g_globalMemoryControl->pool32ElementCount();
}

/// 32バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 32バイト以下のメモリを管理するプールの要素数です。
void GlobalMemoryManager::pool32ElementCount(USize count) noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    g_globalMemoryControl->pool32ElementCount(count);
}
        
/// 64バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 64バイト以下のメモリを管理するプールの要素数です。
USize GlobalMemoryManager::pool64ElementCount() noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    return g_globalMemoryControl->pool64ElementCount();
}

/// 64バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 64バイト以下のメモリを管理するプールの要素数です。
void GlobalMemoryManager::pool64ElementCount(USize count) noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    g_globalMemoryControl->pool64ElementCount(count);
}
        
/// 128バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 128バイト以下のメモリを管理するプールの要素数です。
USize GlobalMemoryManager::pool128ElementCount() noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    return g_globalMemoryControl->pool128ElementCount();
}

/// 128バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 128バイト以下のメモリを管理するプールの要素数です。
void GlobalMemoryManager::pool128ElementCount(USize count) noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    g_globalMemoryControl->pool128ElementCount(count);
}
        
/// 256バイト以下のメモリを管理するプールの要素数を取得します。
/// @return 256バイト以下のメモリを管理するプールの要素数です。
USize GlobalMemoryManager::pool256ElementCount() noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    return g_globalMemoryControl->pool256ElementCount();
}

/// 256バイト以下のメモリを管理するプールの要素数を設定します。
/// @param count 256バイト以下のメモリを管理するプールの要素数です。
void GlobalMemoryManager::pool256ElementCount(USize count) noexcept
{
    std::lock_guard<std::mutex> lock(g_globalMemoryControlMutex);
    std::call_once(g_initGlobalMemoryFlag, initGlobalMemory);
    g_globalMemoryControl->pool256ElementCount(count);
}