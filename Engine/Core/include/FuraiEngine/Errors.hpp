/// @file FuraiEngine/Errors.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// 基本例外を提供します。
/// 基本、例外を投げる処理はデバッグ用機能になります。
/// リリース環境では例外処理として使用できない可能性を考慮してください。
#ifndef _FURAIENGINE_ERROR_HPP
#define _FURAIENGINE_ERROR_HPP

#include "FuraiEngine/Exception.hpp"
#include "FuraiEngine/String.hpp"

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    /// 引数が不正な値です。
    DYNAMIC_LINK_CORE class ArgumentException: public IException
    {
        String m_message; // エラーメッセージです。

    public:

        /// メッセージを設定して初期化します。
        /// @param message エラーメッセージです。
        ArgumentException(const String &message) noexcept;

        /// コピーします。
        /// @param origin コピー元です。
        ArgumentException(const ArgumentException &origin) noexcept;

        /// ムーブします。
        /// @param origin ムーブ元です。
        ArgumentException(const ArgumentException &origin) noexcept;

        /// コピー代入します。
        /// @param origin コピー元です。
        ArgumentException& operator=(const ArgumentException &origin) noexcept;

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        ArgumentException& operator=(const ArgumentException &origin) noexcept;

        /// エラーメッセージを取得します。
        /// @return エラーメッセージです。
        const Char *message() const noexcept;
    };

    /// 参照がnullptrです。
    DYNAMIC_LINK_CORE class NullReferenceException: public IException
    {
        String m_message; // エラーメッセージです。

    public:

        /// メッセージを設定して初期化します。
        /// @param message エラーメッセージです。
        NullReferenceException(const String &message) noexcept;

        /// コピーします。
        /// @param origin コピー元です。
        NullReferenceException(const NullReferenceException &origin) noexcept;

        /// ムーブします。
        /// @param origin ムーブ元です。
        NullReferenceException(const NullReferenceException &origin) noexcept;

        /// コピー代入します。
        /// @param origin コピー元です。
        NullReferenceException& operator=(const NullReferenceException &origin) noexcept;

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        NullReferenceException& operator=(const NullReferenceException &origin) noexcept;

        /// エラーメッセージを取得します。
        /// @return エラーメッセージです。
        const Char *message() const noexcept;
    };

    /// バッファの範囲外です。
    DYNAMIC_LINK_CORE class BufferOverException: public IException
    {
        String m_message; // エラーメッセージです。

    public:

        /// メッセージを設定して初期化します。
        /// @param message エラーメッセージです。
        BufferOverException(const String &message) noexcept;

        /// コピーします。
        /// @param origin コピー元です。
        BufferOverException(const BufferOverException &origin) noexcept;

        /// ムーブします。
        /// @param origin ムーブ元です。
        BufferOverException(const BufferOverException &origin) noexcept;

        /// コピー代入します。
        /// @param origin コピー元です。
        BufferOverException& operator=(const BufferOverException &origin) noexcept;

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        BufferOverException& operator=(const BufferOverException &origin) noexcept;

        /// エラーメッセージを取得します。
        /// @return エラーメッセージです。
        const Char *message() const noexcept;
    };
}

#endif // !_FURAIENGINE_ERROR_HPP