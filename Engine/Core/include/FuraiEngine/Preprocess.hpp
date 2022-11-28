/// @file FuraiEngine/Preprocess.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito.
/// シンボル、マクロを提供します。
#ifndef _FURAIENGINE_PREPROCESS_HPP
#define _FURAIENGINE_PREPROCESS_HPP

#if defined(_MSC_VER) && !defined(__gcc__) && !defined(__clang__)
/// VisualC++の場合'__declspec(dllexport)'を付加します。
#define EXPORT __declspec(dllexport)
/// VisualC++の場合'__declspec(dllimport)'を付加します。
#define IMPORT __declspec(dllimport)
#else
/// VisualC++の場合'__declspec(dllexport)'を付加します。
#define EXPORT
/// VisualC++の場合'__declspec(dllimport)'を付加します。
#define IMPORT
#endif

/// 関数を外部に公開します。
#define DYNAMIC_EXPORT extern "C" EXPORT

/// 外部の関数をロードします。
#define DYNAMIC_IMPORT extern "C" IMPORT

/// 標準文字列にエンコードします。
#define TXT(S) u8##S

#endif // !_FURAIENGINE_PREPROCESS_HPP