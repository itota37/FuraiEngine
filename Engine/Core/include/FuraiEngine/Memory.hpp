/// @file FuraiEngine/Memory.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito.
/// メモリシステムを提供します。
#ifndef _FURAIENGINE_MEMORY_HPP
#define _FURAIENGINE_MEMORY_HPP
#include "FuraiEngine/Utility.hpp"
/// FuraiEngineのすべての機能を含む名前空間です。
namespace FuraiEngine
{
    /// メモリ確保に失敗した場合のエラー型です。
    enum class EBadAllocatedError : U8
    {
        /// 要求サイズが0でした。
        ZERO_SIZE,
        /// ヒープ上にメモリを確保できんせんでした。
        /// メモリ不足の可能性があります。
        BAD_ALLOCATED_MEMORY,
    };

    /// ヒープメモリを確保します。
    /// @param size 確保するメモリサイズです。
    /// @return 確保したメモリのポインタ、または、エラー値です。
    Result<void *, EBadAllocatedError> Allocate(USize size) noexcept;

    /// メモリ解放に失敗した場合のエラー型です。
    enum class EBadDeallocatedError : U8
    {
        /// ポインタがヌルでした。
        NULL_REFERENCE,
        /// 解放サイズが0でした。
        ZERO_SIZE,
        /// ヒープ上のメモリを解放できんせんでした。
        /// 指定のポインタは別のメモリシステムが管理している可能性があります。
        BAD_DEALLOCATED_MEMORY,
    };

    /// ヒープメモリを解放します。
    /// @param pointer 解放するポインタです。
    /// @param size 解放するポインタのメモリサイズです。
    /// @return 成功値、または、エラー値です。
    Result<Success, EBadAllocatedError>
    Deallocate(void *pointer, USize size) noexcept;

    /// 標準アロケータ型です。
    /// @tparam T 要素の型です。
    template<typename T>
    class Allocator
    {
    public:
        /// 要素の型です。
        using ElementType = T;
        /// メモリ確保エラー型です。
        using BadAllocatedErrorType = EBadAllocatedError;
        /// メモリ解放エラー型です。
        using BadDeallocatedErrorType = EBadDeallocatedError;

        /// 初期化します。
        constexpr Allocator() noexcept
        {}

        /// コピーします。
        constexpr Allocator(const Allocator<T> &) noexcept
        {}

        /// ムーブします。
        constexpr Allocator(Allocator<T> &&) noexcept
        {}

        /// コピー代入します。
        constexpr Allocator<T> &operator=(const Allocator<T> &) noexcept
        {
            return *this;
        }

        /// ムーブ代入します。
        constexpr Allocator<T> &operator=(Allocator<T> &&) noexcept
        {
            return *this;
        }

        /// メモリを確保します。
        /// @param size 確保する要素数です。
        /// @return 確保したメモリのポインタ、または、エラー値です。
        Result<ElementType *, BadAllocatedErrorType>
        Allocate(Usize count) noexcept
        {
            return (ElementType *) FuraiEngine::Allocate(
                sizeof(ElementType) * count);
        }

        /// メモリを解放します。
        /// @param pointer 解放するポインタです。
        /// @param count 解放する要素数です。
        /// @return 成功値、または、エラー値です。
        Result<Success, BadAllocatedErrorType>
        Deallocate(ElementType *pointer, USize count) noexcept
        {
            return FuraiEngine::Deallocate((void *) pointer, count);
        }
    };

    /// 標準アロケータのインスタンスです。
    /// @tparam T 要素の型です。
    template<typename T>
    constexpr Allocator<T> ALLOCATOR = Allocator<T>();
}
#endif // !_FURAIENGINE_MEMORY_HPP