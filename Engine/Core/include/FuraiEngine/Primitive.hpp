/// @file FuraiEngine/Primitive.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito.
/// 基本型を提供します。
#ifndef _FURAIENGINE_PRIMITIVE_HPP
#define _FURAIENGINE_PRIMITIVE_HPP

#include <limits>
#include "FuraiEngine/Preprocess.hpp"

/// FuraiEngineのすべての機能を含む名前空間です。
namespace FuraiEngine
{

    /// 符号付き8ビット整数型の型エイリアスです。
    using I8 = signed char;

    /// 符号無し8ビット整数型の型エイリアスです。
    using U8 = unsigned char;
    
    /// 符号付き16ビット整数型の型エイリアスです。
    using I16 = signed short;
    
    /// 符号無し16ビット整数型の型エイリアスです。
    using U16 = unsigned short;
    
    /// 符号付き32ビット整数型の型エイリアスです。
    using I32 = signed int;
    
    /// 符号無し32ビット整数型の型エイリアスです。
    using U32 = unsigned int;
    
    /// 符号付き64ビット整数型の型エイリアスです。
    using I64 = signed long long;
    
    /// 符号無し64ビット整数型の型エイリアスです。
    using U64 = unsigned long long;
    
    /// 32ビット浮動小数点型の型エイリアスです。
    using F32 = float;
    
    /// 64ビット浮動小数点型の型エイリアスです。
    using F64 = double;

    /// 論理型の型エイリアスです。
    using Bool = bool;

#if __cplusplus < 202002L
    /// 標準文字型の型エイリアスです。
    using Char = char;
#else
    /// 標準文字型の型エイリアスです。
    using Char = char8_t;
#endif

    /// アドレス値の表現に十分な符号付き整数型の型エイリアスです。
    using ISize = std::ptrdiff_t;

    /// アドレス値の表現に十分な符号無し整数型の型エイリアスです。
    using USize = std::size_t;

    /// I8型の最小値です。
    constexpr I8 I8_MIN = std::numeric_limits<I8>::min();
    
    /// I8型の最大値です。
    constexpr I8 I8_MAX = std::numeric_limits<I8>::max();

    /// U8型の最小値です。
    constexpr U8 U8_MIN = std::numeric_limits<U8>::min();
    
    /// U8型の最大値です。
    constexpr U8 U8_MAX = std::numeric_limits<U8>::max();

    /// I16型の最小値です。
    constexpr I16 I16_MIN = std::numeric_limits<I16>::min();
    
    /// I16型の最大値です。
    constexpr I16 I16_MAX = std::numeric_limits<I16>::max();

    /// U16型の最小値です。
    constexpr U16 U16_MIN = std::numeric_limits<U16>::min();
    
    /// U16型の最大値です。
    constexpr U16 U16_MAX = std::numeric_limits<U16>::max();

    /// I32型の最小値です。
    constexpr I32 I32_MIN = std::numeric_limits<I32>::min();
    
    /// I32型の最大値です。
    constexpr I32 I32_MAX = std::numeric_limits<I32>::max();

    /// U32型の最小値です。
    constexpr U32 U32_MIN = std::numeric_limits<U32>::min();
    
    /// U32型の最大値です。
    constexpr U32 U32_MAX = std::numeric_limits<U32>::max();

    /// I64型の最小値です。
    constexpr I64 I64_MIN = std::numeric_limits<I64>::min();
    
    /// I64型の最大値です。
    constexpr I64 I64_MAX = std::numeric_limits<I64>::max();

    /// U64型の最小値です。
    constexpr U64 U64_MIN = std::numeric_limits<U64>::min();
    
    /// U64型の最大値です。
    constexpr U64 U64_MAX = std::numeric_limits<U64>::max();

    /// F32型の誤差値です。
    constexpr F32 F32_EPSILON = std::numeric_limits<F32>::epsilon();

    /// F32型の正の無限大です。
    constexpr F32 F32_INFINITY = std::numeric_limits<F32>::infinity();
    
    /// F32型の負の無限大です。
    constexpr F32 F32_NEGATIVE_INFINITY = -F32_INFINITY;

    /// F64型の誤差値です。
    constexpr F64 F64_EPSILON = std::numeric_limits<F64>::epsilon();

    /// F64型の正の無限大です。
    constexpr F64 F64_INFINITY = std::numeric_limits<F64>::infinity();
    
    /// F64型の負の無限大です。
    constexpr F64 F64_NEGATIVE_INFINITY = -F64_INFINITY;

    /// ISize型の最小値です。
    constexpr ISize ISIZE_MIN = std::numeric_limits<ISize>::min();
    
    /// ISize型の最大値です。
    constexpr ISize ISIZE_MAX = std::numeric_limits<ISize>::max();

    /// USize型の最小値です。
    constexpr USize USIZE_MIN = std::numeric_limits<USize>::min();
    
    /// USize型の最大値です。
    constexpr USize USIZE_MAX = std::numeric_limits<USize>::max();

    /// 誤差を考慮して等しいか比較します。
    /// @param l 比較対象です。
    /// @param r 比較対象です。
    /// @retval true 差が誤差の範囲内です。
    /// @retval false 差が誤差の範囲外です。
    inline Bool Approximately(F32 l, F32 r) noexcept
    {
        return fabsf(l - r) <= F32_EPSILON * fmaxf(1.0f, fmaxf(fabsf(l), fabsf(r)));
    }

    /// 誤差を考慮して等しいか比較します。
    /// @param l 比較対象です。
    /// @param r 比較対象です。
    /// @retval true 差が誤差の範囲内です。
    /// @retval false 差が誤差の範囲外です。
    inline Bool Approximately(F64 l, F64 r) noexcept
    {
        return fabs(l - r) <= F64_EPSILON * fmax(1.0, fmax(fabs(l), fabs(r)));
    }

}

#endif // !_FURAIENGINE_PRIMITIVE_HPP