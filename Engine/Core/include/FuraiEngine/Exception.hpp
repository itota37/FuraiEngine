/// @file FuraiEngine/Exception.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// 例外インタフェースを提供します。
/// 基本例外はErrors.hppを参照してください。
/// 基本、例外を投げる処理はデバッグ用機能になります。
/// リリース環境では例外処理として使用できない可能性を考慮してください。
#ifndef _FURAIENGINE_EXCEPTION_HPP
#define _FURAIENGINE_EXCEPTION_HPP

#include "FuraiEngine/Primitive.hpp"

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    /// 例外インタフェースです。
    class IException
    {
    public:
    
        /// エラーメッセージを取得します。
        /// @return エラーメッセージです。
        virtual const Char *message() noexcept = 0;
    };
}

#endif // !_FURAIENGINE_EXCEPTION_HPP