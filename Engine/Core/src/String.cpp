/// @file FuraiEngine/String.cpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// String.hppの定義です。

#include "FuraiEngine/String.hpp"
// #include "FuraiEngine/HashContainer.hpp"

using namespace FuraiEngine;

// --------------------
//
// String
//
// ====================

struct StringInfo
{
    USize referenceCount;
    Char *rawString;
};

// HashMap<String, StringInfo> *g_stringMap;
// std::mutex g_stringMapMutex;
// std::once_flag g_initStringMapFlag;
// void initStringMap()
// {
//     auto ptr = std::malloc(sizeof(HashMap<String, StringInfo>));
//     g_stringMap = new(ptr) HashMap<String, StringInfo>();
// }