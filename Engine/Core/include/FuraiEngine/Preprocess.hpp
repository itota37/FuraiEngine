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

/// 外部呼び出しの為のキーワードを付加します。
#define DYNAMIC_CALL extern "C" DLL_EXPORT

#ifdef FURAIENGINE_CORE
/// コアモジュールの動的リンクの為のキーワードを付加します。
#define DYNAMIC_LINK_CORE DLL_EXPORT
#else
/// コアモジュールの動的リンクの為のキーワードを付加します。
#define DYNAMIC_LINK_CORE DLL_IMPORT
#endif 

#endif // !_FURAIENGINE_PREPROCESS_HPP