/// @file FuraiEngine/Function.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito.
/// 関数オブジェクトを提供します。
#ifndef _FURAIENGINE_FUNCTION_HPP
#define _FURAIENGINE_FUNCTION_HPP
#include <functional>
/// FuraiEngineのすべての機能を含む名前空間です。
namespace FuraiEngine
{
    /// 関数の引数を部分的に束縛します。
    /// @tparam F 関数型です。
    /// @tparam Args 引数の可変長型です。
    /// @param func 関数です。
    /// @param args 可変長引数です。
    /// @return 引数を部分的に束縛した関数です。
    template<typename F, typename...Args>
    auto Bind(F &&func, Args&&...args) noexcept
    {
        return ats::bind(func, args...);
    }

    /// 関数オブジェクト型エイリアスです。
    /// @tparam F 関数シグニチャです。
    template<typename F>
    using Func = std::function<F>;

}
#endif // !_FURAIENGINE_FUNCTION_HPP