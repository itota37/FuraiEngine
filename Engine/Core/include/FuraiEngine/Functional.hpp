/// @file FuraiEngine/Functional.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// 関数オブジェクトを提供します。
#ifndef _FURAIENGINE_FUNCTIONAL_HPP
#define _FURAIENGINE_FUNCTIONAL_HPP

#include <functional>

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    /// 関数オブジェクト型の型エイリアスです。
    template<typename F> using Func = std::function<F>;

    /// 引数をバインドします。
    template<typename...Args>
    auto bind(Args...args)
    {
        return std::bind(args);
    }
}

#endif // !_FURAIENGINE_FUNCTIONAL_HPP