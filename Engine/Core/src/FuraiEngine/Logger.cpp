// FuraiEngine/Logger.cpp
// (C) 2022 FuraiEngineCommunity.
// author Taichi Ito.

#include <iostream>
#include <ctime>
#include <mutex>
#include <new>
#include "FuraiEngine/Logger.hpp"

using namespace FuraiEngine;

// --------------------
//
// ログシステム
//
// ====================

const Char *LOG_FILE_NAME = TXT("Log.txt");
U8 g_logSystemBuffer[sizeof(Logger)];
Logger *g_pLogSystem = nullptr;
std::once_flag g_initializeLogSystemOnceF;
// ログシステムを初期化します。
void InitializeLogSystem()
{
    g_pLogSystem = new(g_logSystemBuffer) Logger(LOG_FILE_NAME);
}
// ログシステムを終了化します。
struct FinalizeLogSystem
{
    ~FinalizeLogSystem()
    {
        if (g_pLogSystem)
        {
            g_pLogSystem->~Logger();
            g_pLogSystem = nullptr;
        }
    }
}g_finalizeLogSystem;

// --------------------
//
// ログインタフェース
//
// ====================

// ログを出力します。
//message 出力メッセージです。
void FuraiEngine::Log(const Char *message) noexcept
{
    std::call_once(g_initializeLogSystemOnceF, InitializeLogSystem);
    g_pLogSystem->Write(message);
}

// 警告を出力します。
// message 出力メッセージです。
void FuraiEngine::LogWarning(const Char *message) noexcept
{
    std::call_once(g_initializeLogSystemOnceF, InitializeLogSystem);
    g_pLogSystem->WriteWarning(message);
}

// エラーを出力します。
// message 出力メッセージです。
void FuraiEngine::LogError(const Char *message) noexcept
{
    std::call_once(g_initializeLogSystemOnceF, InitializeLogSystem);
    g_pLogSystem->WriteError(message);
}

// --------------------
//
// Logger
//
// ====================

// ログを出力します。
// level 出力ラベルです。
// message 出力メッセージです。
void FuraiEngine::Logger::m_Write(const Char *label, const Char *message) noexcept
{
    if (this->m_pFile == nullptr)
    {
        std::cerr << "Not initialized Logger" << std::endl;
        return;
    }
    __time64_t longTime;
    _time64(&longTime); // 時間を取得
    tm * localTime = _localtime64(&longTime); // ローカル時間に変換
    fprintf( // 内容を設定
        this->m_pFile,
        TXT("[ %02d/%02d - %02d:%02d:%02d ] %s : %s\n"),
        localTime ->tm_mon + 1, 
        localTime ->tm_mday,
        localTime ->tm_hour, 
        localTime ->tm_min, 
        localTime ->tm_sec,
        label,
        message
    );
    fflush(this->m_pFile); // 書き込み
}

// 初期化します。
// fileName ログファイル名です。
FuraiEngine::Logger::Logger(const Char *fileName) noexcept
{
    this->m_pFile = fopen(fileName, "w");
    if (m_pFile == nullptr)
    {
        std::cerr << "Failed to create or open the file " << fileName << std::endl;
    }
}

// 終了化します。
FuraiEngine::Logger::~Logger() noexcept
{
    if (this->m_pFile != nullptr)
    {
        fclose(this->m_pFile);
    }
}

// ログを出力します。
// message 出力メッセージです。
void FuraiEngine::Logger::Write(const Char *message) noexcept
{
    this->m_Write(TXT("LOG"), message);
}

// 警告を出力します。
// message 出力メッセージです。
void FuraiEngine::Logger::WriteWarning(const Char *message) noexcept
{
    this->m_Write(TXT("WARNING"), message);
}

// エラーを出力します。
// message 出力メッセージです。
void FuraiEngine::Logger::WriteError(const Char *message) noexcept
{
    this->m_Write(TXT("ERROR"), message);
}