// Utility.cpp
// (C) 2022 FuraiEngineCommunity.
// author Taichi Ito.

#include <ctime>
#include <iostream>
#include <mutex>
#include <new>
#include "FuraiEngine/Utility.hpp"

using namespace FuraiEngine;
using namespace FuraiEngine::_Internal;

// --------------------
//
// ログシステム
//
// ====================

const Char *LOG_FILE_NAME = TXT("Log.txt"); // ログファイル名です。
FILE *g_pFile = nullptr; // ログファイルのポインタです。
std::mutex g_logMutexF; // ログ書き込みのための占有ロックフラグです。
std::once_flag g_initializeLogSystemOnceF; // ログシステムの初期化フラグです。
// ログシステムを解体します。
void FinalizeLogSystem()
{
    fclose(g_pFile);
}
// ログシステムを初期化します。
void InitializeLogSystem()
{
    if (fopen_s(&g_pFile, LOG_FILE_NAME, "w") != 0)
    {
        std::cerr << "Failed to create or open the file "
                  << LOG_FILE_NAME << "." << std::endl;
    }
    else
    {
        std::atexit(FinalizeLogSystem);
    }
}

// --------------------
//
// Logger
//
// ====================

// 初期化します。
// label メッセージのカテゴリラベルです。
// 同スレッド内でインスタンス解放前にもう1つインスタンス化するとデッドロックが発生します。
FuraiEngine::_Internal::Logger::Logger(const Char *label) noexcept
{
    std::call_once(g_initializeLogSystemOnceF, InitializeLogSystem);
    if (g_pFile == nullptr)
        return;

    g_logMutexF.lock(); // 占有ロック
    __time64_t longTime;
    _time64(&longTime); // 時間を取得
    tm localTime;
    if (_localtime64_s(&localTime, &longTime) != 0) // ローカル時間に変換
    {
        fprintf(g_pFile, TXT("[ \?\?/\?\? - \?\?:\?\?:\?\? ] %s : "), label);
    }
    else
    {
        fprintf( // 内容を設定
            g_pFile,
            TXT("[ %02d/%02d - %02d:%02d:%02d ] %s : "),
            localTime.tm_mon + 1,
            localTime.tm_mday,
            localTime.tm_hour,
            localTime.tm_min,
            localTime.tm_sec,
            label);
    }
}

// ムーブします。
// origin ムーブ元です。
FuraiEngine::_Internal::Logger::Logger(Logger &&origin) noexcept
{}

// 解体します。
FuraiEngine::_Internal::Logger::~Logger() noexcept
{
    if (g_pFile == nullptr)
        return;

    fprintf(g_pFile, "%s", TXT("\n"));
    fflush(g_pFile);      // 書き込み
    g_logMutexF.unlock(); // 占有ロック解除
}

// メッセージを追加します。
// message メッセージです。
// 自身のインスタンスです。
Logger &&FuraiEngine::_Internal::Logger::Wright(const Char *message) noexcept
{
    if (g_pFile != nullptr)
        fprintf(g_pFile, "%s", message);
    return Move(*this);
}

// --------------------
//
// ログインタフェース
//
// ====================

// ログを出力します。
//message 出力メッセージです。
void FuraiEngine::Log(const Char *message) noexcept
{
    Logger(LOG_LABEL).Wright(message);
}

// 警告を出力します。
// message 出力メッセージです。
void FuraiEngine::LogWarning(const Char *message) noexcept
{
    Logger(WARNING_LABEL).Wright(message);
}

// エラーを出力します。
// message 出力メッセージです。
void FuraiEngine::LogError(const Char *message) noexcept
{
    Logger(ERROR_LABEL).Wright(message);
}