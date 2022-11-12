/// @file FuraiEngine/Preprocess.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// シンボルとマクロを提供します。
#ifndef _FURAIENGINE_PREPROCESS_HPP
#define _FURAIENGINE_PREPROCESS_HPP

#if defined(_MSC_VER) && !defined(__clang__) && !defined(__gcc__)
/// VisualC++の場合に__declspec(dllexport)を付加します。
#define DLL_EXPORT __declspec(dllexport)
/// VisualC++の場合に__declspec(dllimport)を付加します。
#define DLL_IMPORT __declspec(dllimport)
#else
/// VisualC++の場合に__declspec(dllexport)を付加します。
#define DLL_EXPORT 
/// VisualC++の場合に__declspec(dllimport)を付加します。
#define DLL_IMPORT 
#endif

#ifdef FURAIENGINE_CORE
/// 動的リンクの為のキーワードを付加します。
#define DYNAMIC_LINK DLL_EXPORT
/// 動的ロードの為のキーワードを付加します。
#define DYNAMIC_LOAD extern "C" DLL_EXPORT
#else
/// 動的リンクの為のキーワードを付加します。
#define DYNAMIC_LINK DLL_IMPORT
/// 動的ロードの為のキーワードを付加します。
#define DYNAMIC_LOAD extern "C" DLL_IMPORT
#endif 

#endif // !_FURAIENGINE_PREPROCESS_HPP