// FuraiEngine/Logger.hpp
// (C) 2022 FuraiEngineCommunity.
// author Taichi Ito.
// ログ機能を提供します。
#ifndef _FURAIENGINE_LOGGER_HPP
#define _FURAIENGINE_LOGGER_HPP

#include <cstdio>
#include "FuraiEngine/Utility.hpp"

// FuraiEngineのすべての機能を含む名前空間です。
namespace FuraiEngine
{

    // ログシステムです。
    class Logger
    {
        FILE *m_pFile;

        // ログを出力します。
        // level 出力ラベルです。
        // message 出力メッセージです。
        void m_Write(const Char *label, const Char *message) noexcept;

    public:

        // 初期化します。
        // fileName ログファイル名です。
        Logger(const Char *fileName) noexcept;

        // 終了化します。
        ~Logger() noexcept;

        // ログを出力します。
        // message 出力メッセージです。
        void Write(const Char *message) noexcept;

        // 警告を出力します。
        // message 出力メッセージです。
        void WriteWarning(const Char *message) noexcept;

        // エラーを出力します。
        // message 出力メッセージです。
        void WriteError(const Char *message) noexcept;
    };

}

#endif // !_FURAIENGINE_LOGGER_HPP