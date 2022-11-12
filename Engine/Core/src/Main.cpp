/// @file Test.cpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// テストのエントリポイント、または、実行ファイルからの呼び出し位置を提供します。

#include <iostream>
#include "FuraiEngine/Allocation.hpp"

using namespace FuraiEngine;

#ifdef FURAIENGINE_TEST
#define PRINT(M) std::cout << M
#define PRINT_LN(M) PRINT(M) << std::endl
// テストを実行します
int main()
{
    PRINT_LN("FuraiEngineCoreのテストを実行します。");
    PRINT_LN("FuraiEngineCoreのテストを終了します。");
    return 0;
}
# elif _FURAIENGINE_CORE_DYNAMIC
/// 実行ファイルから呼び出されるエンジンのエントリポイントです。
DYNAMIC_LOAD int run()
{
    return 0;
}
#endif