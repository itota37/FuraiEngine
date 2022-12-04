// Memory.cpp
// (C) 2022 FuraiEngineCommunity.
// author Taichi Ito.

#include <new>
#include "FuraiEngine/Collections/Array.hpp"
#include "FuraiEngine/Memory.hpp"

using namespace FuraiEngine;

// --------------------
//
// メモリシステム
//
// ====================

/// メモリプールです。
/// @tparam ELEMENT_SIZE 1要素のサイズです。
/// @tparam ELEMENTS_COUNT このプールが管理する要素数です。
template<USize ELEMENT_SIZE, USize ELEMENTS_COUNT>
class MemoryPool
{
public:
    /// バッファのサイズです。
    static constexpr USize BUFFER_SIZE = ELEMENT_SIZE * ELEMENTS_COUNT;

private:
    U8 m_buffer[BUFFER_SIZE]; // このプールが管理するメモリバッファです。
    U8 **m_ppElementListTop; // 要素の単方向連結リストの先頭です。
    USize m_currentElementsCount; // 現在の要素数です。
    USize m_bufferAddressMin; // バッファ範囲の最小アドレス値です。
    USize m_bufferAddressMaxOver; // バッファ範囲の超過アドレス値です。

public:
    /// 初期化します。
    MemoryPool() noexcept
        : m_ppElementListTop(nullptr)
        , m_currentElementsCount(ELEMENTS_COUNT)
        , m_bufferAddressMin((USize) &m_buffer[0])
        , m_bufferAddressMaxOver((USize) &m_buffer[BUFFER_SIZE])
    {
        // 要素のリストを作成します
        //
        //   m_buffer [elem][elem][elem]...
        //            |  ^  |  ^  |  ^
        // nullptr <--'  '--'  '--'  '-- m_ppElementListTop
        //
        for (USize i = 0; i < BUFFER_SIZE; i += ELEMENT_SIZE)
        {
            auto ptr = (U8 **) &this->m_buffer[i];
            *ptr     = (U8 *) this->m_ppElementListTop;
            this->m_ppElementListTop = ptr;
        }
    }

    /// 要素を取り出します。
    /// @return 取り出した要素です。
    void *Allocate() noexcept
    {
        this->m_currentElementsCount -= 1;

        auto ptr                 = this->m_ppElementListTop;
        this->m_ppElementListTop = (U8 **) *ptr;
        return (void *) ptr;
    }

    /// 要素を戻します。
    /// @param pointer 戻す要素のポインタです。
    void Deallocate(void *pointer) noexcept
    {
        this->m_currentElementsCount += 1;

        auto ptr = (U8 **) pointer;
        *ptr     = (U8 *) this->m_ppElementListTop;
        this->m_ppElementListTop = ptr;
    }

    /// 現在取り出し可能な要素の数を取得します。
    /// @return 現在取り出し可能な要素の数です。
    USize CurrentElementsCount() noexcept
    {
        return this->m_currentElementsCount;
    }

    /// バッファの最小アドレスを取得します。
    /// @return バッファの最小アドレスです。
    USize MinAddress() noexcept
    {
        return this->m_bufferAddressMin;
    }

    /// 指定のアドレスがバッファに含まれるか判定します。
    /// @param address 判定するアドレスです。
    /// @return 含まれていた時、真です。
    Bool ManagedAddressFor(USize address) noexcept
    {
        return this->m_bufferAddressMin <= address
            && address < this->m_bufferAddressMaxOver;
    }
};

/// システムアロケータ型です。
/// @tparam T 要素の型です。
template<typename T>
class SystemAllocator
{
public:
    /// 要素の型です。
    using ElementType = T;
    /// メモリ確保エラー型です。
    using BadAllocatedErrorType = EBadAllocatedError;
    /// メモリ解放エラー型です。
    using BadDeallocatedErrorType = EBadDeallocatedError;

    /// 初期化します。
    constexpr SystemAllocator() noexcept
    {}

    /// コピーします。
    constexpr SystemAllocator(const SystemAllocator<T> &) noexcept
    {}

    /// ムーブします。
    constexpr SystemAllocator(SystemAllocator<T> &&) noexcept
    {}

    /// コピー代入します。
    constexpr SystemAllocator<T> &
    operator=(const SystemAllocator<T> &) noexcept
    {
        return *this;
    }

    /// ムーブ代入します。
    constexpr SystemAllocator<T> &operator=(SystemAllocator<T> &&) noexcept
    {
        return *this;
    }

    /// メモリを確保します。
    /// @param size 確保する要素数です。
    /// @return 確保したメモリのポインタ、または、エラー値です。
    Result<ElementType *, BadAllocatedErrorType>
    Allocate(Usize count) noexcept
    {
        ElementType *ptr = std::malloc(sizeof(ElementType) * count);
        if (ptr != nullptr)
            return ptr;
        else
            return BadAllocatedErrorType::BAD_ALLOCATED_MEMORY;
    }

    /// メモリを解放します。
    /// @param pointer 解放するポインタです。
    /// @param count 解放する要素数です。
    /// @return 成功値、または、エラー値です。
    Result<Success, BadAllocatedErrorType>
    Deallocate(ElementType *pointer, USize count) noexcept
    {
        static_cast<int>(count); // 警告を回避します。
        std::free(pointer);
        return SUCCESS;
    }
};

/// 固定長メモリシステムです。
/// @tparam ELEMENT_SIZE 1要素のサイズです。
/// @tparam ONE_POOL_ELEMENTS_COUNT 1つのプールが管理する要素数です。
template<USize ELEMENT_SIZE, USize ONE_POOL_ELEMENTS_COUNT>
class FixedMemorySystem
{
    Array<MemoryPool *, SystemAllocator<MemoryPool *>> m_pools; // メモリプールのリストです。
    MemoryPool *m_pAllocateTargetPool; // メモリ確保対象のメモリプールです。

    /// メモリプールを作成します。
    /// @return 作成したメモリプールのインスタンス、または、ヌルです。
    MemoryPool *_CreatePool() noexcept
    {
        if (auto ptr = std::malloc(sizeof(MemoryPool)))
            return new (ptr)
                MemoryPool<ELEMENT_SIZE, ONE_POOL_ELEMENTS_COUNT>();
        else
            return (MemoryPool *) nullptr;
    }

    /// メモリプールを解放します。
    /// @param pointer 解放するメモリプールのインスタンスです。
    void _DestroyPool(MemoryPool *pointer) noexcept
    {
        std::free(pointer);
    }

    /// メモリプールを追加し、メモリ確保対象に設定します。

public:
};

// --------------------
//
// 関数
//
// ====================

// ヒープメモリを確保します。
// size 確保するメモリサイズです。
// return 確保したメモリのポインタ、または、エラー値です。
Result<void *, EBadAllocatedError> FuraiEngine::Allocate(USize size) noexcept
{}

// ヒープメモリを解放します。
// pointer 解放するポインタです。
// size 解放するポインタのメモリサイズです。
// return 成功値、または、エラー値です。
Result<Success, EBadAllocatedError>
FuraiEngine::Deallocate(void *pointer, USize size) noexcept
{}