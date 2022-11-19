/// @file FuraiEngine/Set.cpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// Set.hppの定義です。

#include "FuraiEngine/Set.hpp"

using namespace FuraiEngine;

constexpr USize K8 = 81;                   // 1 / pi * 2 ^ 8
constexpr USize K16 = 40503;               // (sqrt(5) - 1) / 2 * (2 ^ 16)
constexpr USize K32 = 2654435769;          // (sqrt(5) - 1) / 2 * (2 ^ 32)
constexpr USize K64 = 5871781006564002453; // 1 / pi * 2 ^ 64

constexpr USize BYTE_BIT = 8;
constexpr USize SHIFT = 5;

USize rotateBitL(USize value, USize step) { 
    return (value << step) | (value >> (sizeof(USize) * BYTE_BIT - step)); 
}

/// ハッシュ値を加算します。
/// @param hash 加算先です。
/// @param value 加算対象です。
void FuraiEngine::addHash(USize &hash, U8 value) noexcept
{
    hash = rotateBitL(hash, SHIFT);
    hash ^= value;
    hash *= K8;
}

/// ハッシュ値を加算します。
/// @param hash 加算先です。
/// @param value 加算対象です。
void FuraiEngine::addHash(USize &hash, U16 value) noexcept
{
    hash = rotateBitL(hash, SHIFT);
    hash ^= value;
    hash *= K16;
}

/// ハッシュ値を加算します。
/// @param hash 加算先です。
/// @param value 加算対象です。
void FuraiEngine::addHash(USize &hash, U32 value) noexcept
{
    hash = rotateBitL(hash, SHIFT);
    hash ^= value;
    hash *= K32;
}

/// ハッシュ値を加算します。
/// @param hash 加算先です。
/// @param value 加算対象です。
void FuraiEngine::addHash(USize &hash, U64 value) noexcept
{
    hash = rotateBitL(hash, SHIFT);
    hash ^= value;
    hash *= K64;
}