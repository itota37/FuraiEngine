/// @file FuraiEngine/Utility.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito.
/// 有用機能を提供します。
#ifndef _FURAIENGINE_UTILITY_HPP
#define _FURAIENGINE_UTILITY_HPP

#include "FuraiEngine/Primitive.hpp"

/// FuraiEngineのすべての機能を含む名前空間です。
namespace FuraiEngine
{

    /// ログを出力します。
    /// @param message 出力メッセージです。
    void Log(const Char *message) noexcept;

    /// 警告を出力します。
    /// @param message 出力メッセージです。
    void LogWarning(const Char *message) noexcept;

    /// エラーを出力します。
    /// @param message 出力メッセージです。
    void LogError(const Char *message) noexcept;

    /// ムーブします。
    /// @tparam T ムーブする型です。
    /// @param value ムーブする値です。
    /// @return ムーブする値です。
    template<typename T>
    constexpr std::remove_reference<T>::type &&Move(T &&value) noexcept
    {
        return std::move(value);
    }

    /// 左辺値はコピー、右辺値はムーブします。
    /// @tparam T 適用する型です。
    /// @param value 適用する値です。
    /// @return 適用する値です。
    template<typename T>
    constexpr T&& Forward(typename std::remove_reference<T>::type &value) noexcept
    {
        return std::forward(value);
    }
    
    /// 左辺値はコピー、右辺値はムーブします。
    /// @tparam T 適用する型です。
    /// @param value 適用する値です。
    /// @return 適用する値です。
    template<typename T>
    constexpr T&& Forward(typename std::remove_reference<T>::type &&value) noexcept
    {
        return std::forward(value);
    }

    /// 戻り値の成功、または、失敗を表現します。
    /// @tparam S 成功時の型です。
    /// @tparam F 失敗時の型です。
    template<typename S, typename F>
    class Result
    {
        union UValue
        {
            U8 m_ready;  // 初期化を遅延させるための、ダミー値です。
            S m_success; // 成功時の値です。
            F m_failur;  // 失敗時の値です。

            /// ダミー値で初期化します。
            UValue()
                : m_ready(0)
            {}

        } m_value;
        enum class EState: U8
        {
            NONE,    // 値はダミー値です。
            SUCCESS, // 値は成功値です。
            FAILUR   // 値は失敗値です。
        } m_state;   // 値の判別値です。

    public:

        /// 空で初期化します。
        Result() noexcept
            : m_value()
            , m_state(EState::NONE)
        {}

        /// 成功値で初期化します。
        /// @param value 成功値です。
        Result(S &&value) noexcept
            : m_value()
            , m_state(EState::SUCCESS)
        {
            this->m_value.m_success = Move(value);
        }

        /// 失敗値で初期化します。
        /// @param value 失敗値です。
        Result(F &&value) noexcept
            : m_value()
            , m_state(EState::FAILUR)
        {
            this->m_value.m_failur = Move(value);
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        /// @return 自身のインスタンスです。
        Result<S, F> &operator=(Result<S, F> &&origin) noexcept
        {
            if (this != &origin)
            {
                auto rrTmpState = Move(this->m_state);
                auto rrTmpValue = Move(this->m_value);
                this->m_state = Move(origin.m_state);
                this->m_value = Move(origin.m_value);
                origin.m_state = Move(rrTmpState);
                origin.m_value = Move(rrTmpValue);
            }
            return *this;
        }

        /// ムーブします。
        /// @param origin ムーブ元です。
        Result(Result<S, F> &&origin) noexcept
            : Result()
        {
            *this = Move(origin);
        }

        /// 終了化します。
        ~Result()
        {
            // デストラクタを有する場合に、
            // 安全にデストラクタを呼びたい

            if (this->m_state == EState::SUCCESS)
            {
                S tmp = this->m_value.m_success;
            }
            else if (this->m_state == EState::FAILUR)
            {
                F tmp = this->m_value.m_failur;
            }
        }
    };
}

#endif // !_FURAIENGINE_UTILITY_HPP