// Test.cpp
// (C) 2022 FuraiEngineCommunity.
// author Taichi Ito.

#include <iostream>
#include <typeinfo>
#include "FuraiEngine/Utility.hpp"

using namespace FuraiEngine;

Result<U32, Bool> ResultTest(Bool b) noexcept
{
    if (b)
    {
        return (U32) 17;
    }
    else
    {
        return true;
    }
}

int main()
{
    std::cout << "Test start." << std::endl;

    //
    // Log
    //
    std::cout << "Test 'Log' start." << std::endl;
    Log(TXT("Test, log! テストです。"));
    LogWarning(TXT("Test, warning! テストです。"));
    LogError(TXT("Test, error! テストです。"));
    std::cout << "Test 'Log' end" << std::endl;

    //
    // Result
    //
    std::cout << "Test 'Result' start." << std::endl;
    U32 resultTrue = 0;
    if (ResultTest(true).IsSuccess(resultTrue))
        std::cout << "Test is successed. " << (U32) 17
                  << " = " << resultTrue << std::endl;
    else
        std::cout << "Test is failed." << std::endl;
    Bool resultFalse = false;
    if (ResultTest(false).IsFailur(resultFalse))
        std::cout << "Test is successed. " << true << " = "
                  << resultFalse << std::endl;
    else
        std::cout << "Test is failed." << std::endl;
    std::cout << "Test 'Result' end" << std::endl;

    std::cout << "Test end" << std::endl;
    return 0;
}