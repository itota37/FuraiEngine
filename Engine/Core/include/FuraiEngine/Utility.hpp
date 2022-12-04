/// @file FuraiEngine/Utility.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito.
/// 有用機能を提供します。
#ifndef _FURAIENGINE_UTILITY_HPP
#define _FURAIENGINE_UTILITY_HPP
#include <typeinfo>
#include "FuraiEngine/Primitive.hpp"
/// FuraiEngineのすべての機能を含む名前空間です。
namespace FuraiEngine
{
    /// @cond FURAIDOC_INTERNAL
    /// 非公開機能を含む名前空間です。
    namespace _Internal
    {
        /// ログ用ラベルです。
        static const Char *LOG_LABEL = TXT("LOG");

        /// 警告用ラベルです。
        static const Char *WARNING_LABEL = TXT("WARNING");

        /// エラー用ラベルです。
        static const Char *ERROR_LABEL = TXT("ERROR");

        /// ロガーです。
        class Logger
        {
        public:
            /// 初期化します。
            /// @param label メッセージのカテゴリラベルです。
            /// @warning 同スレッド内でインスタンス解放前にもう1つインスタンス化するとデッドロックが発生します。
            Logger(const Char *label) noexcept;

            /// ムーブします。
            /// @param origin ムーブ元です。
            Logger(Logger &&origin) noexcept;

            /// 解体します。
            ~Logger() noexcept;

            /// メッセージを追加します。
            /// @param message メッセージです。
            /// @return 自身のインスタンスです。
            Logger &&Write(const Char *message) noexcept;
        };
    }
    /// @endcode

    /// 型名を取得します。
    /// @tparam T 型名を取得したい型です。
    template<typename T>
    const Char *TypenameOf() noexcept
    {
        return typeid(T).name();
    }

    /// ログを出力します。
    /// @param message 出力メッセージです。
    void Log(const Char *message) noexcept;

    /// 警告を出力します。
    /// @param message 出力メッセージです。
    void LogWarning(const Char *message) noexcept;

    /// エラーを出力します。
    /// @param message 出力メッセージです。
    void LogError(const Char *message) noexcept;

    /// 正常終了します。
    [[noreturn]] inline void Exit() noexcept
    {
        std::exit(0);
    }

    /// 異常終了します。
    [[noreturn]] inline void ExitError() noexcept
    {
        std::exit(-1);
    }

    /// 異常終了します。
    /// @param message 出力メッセージです。
    [[noreturn]] inline void ExitError(const Char *message) noexcept
    {
        LogError(message);
        ExitError();
    }

    /// ムーブします。
    /// @tparam T ムーブする型です。
    /// @param value ムーブする値です。
    /// @return ムーブする値です。
    template<typename T>
    constexpr typename std::remove_reference<T>::type &&
    Move(T &&value) noexcept
    {
        return std::move(value);
    }

    /// 左辺値はコピー、右辺値はムーブします。
    /// @tparam T 適用する型です。
    /// @param value 適用する値です。
    /// @return 適用する値です。
    template<typename T>
    constexpr T &&
    Forward(typename std::remove_reference<T>::type &value) noexcept
    {
        return std::forward(value);
    }

    /// 左辺値はコピー、右辺値はムーブします。
    /// @tparam T 適用する型です。
    /// @param value 適用する値です。
    /// @return 適用する値です。
    template<typename T>
    constexpr T &&
    Forward(typename std::remove_reference<T>::type &&value) noexcept
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
            U8 m_ready; // 初期化を遅延させるための、ダミー値です。
            S m_success; // 成功時の値です。
            F m_failur;  // 失敗時の値です。
            /// ダミー値で初期化します。
            UValue(): m_ready(0)
            {}
        } m_value;
        enum class EState : U8
        {
            READY,   // 値はダミー値です。
            SUCCESS, // 値は成功値です。
            FAILUR   // 値は失敗値です。
        } m_state;   // 値の判別値です。

    public:
        /// 成功値で初期化します。
        /// @param value 成功値です。
        Result(S &&value) noexcept
            : m_value(), m_state(EState::SUCCESS)
        {
            this->m_value.m_success = Move(value);
        }

        /// 失敗値で初期化します。
        /// @param value 失敗値です。
        Result(F &&value) noexcept
            : m_value(), m_state(EState::FAILUR)
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
                this->m_state   = Move(origin.m_state);
                this->m_value   = Move(origin.m_value);
                origin.m_state  = Move(rrTmpState);
                origin.m_value  = Move(rrTmpValue);
            }
            return *this;
        }

        /// ムーブします。
        /// @param origin ムーブ元です。
        Result(Result<S, F> &&origin) noexcept
            : m_value(), m_state(EState::READY)
        {
            *this = Move(origin);
        }

        /// 終了化します。
        ~Result()
        {
            // デストラクタを有する場合に、
            // 安全にデストラクタを呼びたい
            if (this->m_state == EState::SUCCESS) [[maybe_unused]]
                S _tmp = Move(this->m_value.m_success);
            else if (this->m_state == EState::FAILUR) [[maybe_unused]]
                F _tmp = Move(this->m_value.m_failur);
        }

        /// 成功か判定します。
        /// @return 成功判定値です。
        Bool IsSuccess() noexcept
        {
            return this->m_state == EState::SUCCESS;
        }

        /// 成功か判定します。
        /// @param value 成功値を受け取る参照です。
        /// @return 成功判定値です。
        Bool IsSuccess(S &value) noexcept
        {
            if (this->IsSuccess())
            {
                value = Move(this->m_value.m_success);
                return true;
            }
            return false;
        }

        /// 成功か判定します。
        /// @param success 成功値を受け取る参照です。
        /// @param failur 失敗値を受け取る参照です。
        /// @return 成功判定値です。
        /// @warning 値の移動後に取り出そうとした場合、異常終了します。
        Bool IsSuccess(S &success, F &failur) noexcept
        {
            if (this->IsSuccess())
            {
                success = Move(this->m_value.m_success);
                return true;
            }
            else if (this->IsFailur())
            {
                failur = Move(this->m_value.m_failur);
                return false;
            }
            else
            {
                _Internal::Logger(_Internal::ERROR_LABEL)
                    .Write(TXT("移動された値にアクセスしよ"
                               "うとしました。'"))
                    .Write(TXT("Result<"))
                    .Write(TypenameOf<S>())
                    .Write(TXT(", "))
                    .Write(TypenameOf<F>())
                    .Write(TXT(">::IsSuccess("))
                    .Write(TypenameOf<S>())
                    .Write(TXT(" &success, "))
                    .Write(TypenameOf<F>())
                    .Write(TXT(" &failur) noexcept'"));

                ExitError();
            }
        }

        /// 失敗か判定します。
        /// @return 失敗判定値です。
        Bool IsFailur() noexcept
        {
            return this->m_state == EState::FAILUR;
        }

        /// 失敗か判定します。
        /// @param value 失敗値を受け取る参照です。
        /// @return 失敗判定値です。
        Bool IsFailur(F &value) noexcept
        {
            if (this->IsFailur())
            {
                value = Move(this->m_value.m_failur);
                return true;
            }
            return false;
        }
    };

    /// 成功を表現する型です。
    class Success
    {
    public:
        /// 初期化します。
        constexpr Success() noexcept
        {}

        /// コピーします。
        constexpr Success(const Success &) noexcept
        {}

        /// ムーブします。
        constexpr Success(Success &&) noexcept
        {}

        /// コピー代入します。
        constexpr Success &operator=(const Success &) noexcept
        {
            return *this;
        }

        /// ムーブ代入します。
        constexpr Success &operator=(Success &&) noexcept
        {
            return *this;
        }
    };

    /// 失敗を表現する型です。
    class Failur
    {
    public:
        /// 初期化します。
        constexpr Failur() noexcept
        {}

        /// コピーします。
        constexpr Failur(const Failur &) noexcept
        {}

        /// ムーブします。
        constexpr Failur(Failur &&) noexcept
        {}

        /// コピー代入します。
        constexpr Failur &operator=(const Failur &) noexcept
        {
            return *this;
        }

        /// ムーブ代入します。
        constexpr Failur &operator=(Failur &&) noexcept
        {
            return *this;
        }
    };

    /// 成功値です。
    constexpr Success SUCCESS = Success();

    /// 失敗値です。
    constexpr Failur FAILUR = Failur();
}
#endif // !_FURAIENGINE_UTILITY_HPP