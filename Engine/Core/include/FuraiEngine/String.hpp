/// @file FuraiEngine/String.hpp
/// @copyright (C) 2022 FuraiEngineCommunity.
/// @author Taichi Ito
/// 文字列を提供します。
#ifndef _FURAIENGINE_STRING_HPP
#define _FURAIENGINE_STRING_HPP

#include <iterator>
#include "FuraiEngine/Primitive.hpp"
#include "FuraiEngine/Exception.hpp"

/// FuraiEngineの機能を提供する名前空間です。
namespace FuraiEngine
{
    
    /// ポインタイテレータです。
    template<typename T>
    class PointerIterator
    {
        T *m_element;

    public:

        /// イテレータのカテゴリフラグ型です。
        using iterator_category = std::random_access_iterator_tag;

        /// 要素型の型エイリアスです。
        using value_type = T;

        /// イテレータの移動距離を表現する為の符号付き整数型の型エイリアスです。
        using difference_type = ISize;

        /// 要素型のポインタ型エイリアスです。
        using pointer = T*;
        
        /// 要素型の参照型エイリアスです。
        using reference = T&;

        /// 初期化します。
        /// @param pointer メモリ上の現在地を指すポインタです。
        PointerIterator(T *pointer) noexcept
        : m_element(pointer)
        {}

        /// コピーします。
        /// @param origin コピー元です。
        PointerIterator(const PointerIterator<T> &origin) noexcept
        : m_element(origin.m_element)
        {}

        /// ムーブします。
        /// @param origin ムーブ元です。
        PointerIterator(PointerIterator<T> &&origin) noexcept
        : m_element(origin.m_element)
        {
            origin.m_element = nullptr;
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        PointerIterator<T>& operator=(const PointerIterator<T> &origin) noexcept
        {
            this->m_element = origin.m_element;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        PointerIterator<T>& operator=(PointerIterator<T> &&origin) noexcept
        {
            this->m_element = origin.m_element;
            origin.m_element = nullptr;
            return *this;
        }

        /// 指定分進めます。
        /// @param step 進める数です。
        PointerIterator<T>& operator+=(USize step) noexcept
        {
            this->m_element += step;
            return *this;
        }

        /// 指定分戻ります。
        /// @param step 戻る数です。
        PointerIterator<T>& operator-=(USize step) noexcept
        {
            this->m_element -= step;
            return *this;
        }
        
        /// 一つ進めます。
        PointerIterator<T>& operator++() noexcept
        {
            this->m_element += 1;
            return *this;
        }
        
        /// 一つ進めて、進む前のイテレータを返します。
        /// @return 進める前のイテレータです。
        PointerIterator<T> operator++(int) noexcept
        {
            auto iter = *this;
            this->m_element += 1;
            return iter;
        }
        
        /// 一つ戻ります。
        PointerIterator<T>& operator--() noexcept
        {
            this->m_element -= 1;
            return *this;
        }
        
        /// 一つ戻って、戻る前のイテレータを返します。
        /// @return 戻る前のイテレータです。
        PointerIterator<T> operator--(int) noexcept
        {
            auto iter = *this;
            this->m_element -= 1;
            return iter;
        }

        /// 要素にアクセスします。
        /// @exception NullRefarenceException 要素がnullptrの可能性があります。
        T &operator*();

        /// 要素にアクセスします。
        /// @exception NullRefarenceException 要素がnullptrの可能性があります。
        const T &operator*() const;

        /// 要素が同等か比較します。
        /// @param other 比較対象です。
        /// @return 同等の場合、真です。
        Bool operator==(const PointerIterator<T> &other) const noexcept
        {
            return l.m_element == r.m_element;
        }

        /// 要素が不等か比較します。
        /// @param other 比較対象です。
        /// @return 不等の場合、真です。
        Bool operator!=(const PointerIterator<T> &other) const noexcept
        {
            return l.m_element != r.m_element;
        }
    };

    /// 不変ポインタイテレータです。
    template<typename T>
    class ConstPointerIterator
    {
        T *m_element;
    
    public:

        /// イテレータのカテゴリフラグ型です。
        using iterator_category = std::random_access_iterator_tag;

        /// 要素型の型エイリアスです。
        using value_type = const T;

        /// イテレータの移動距離を表現する為の符号付き整数型の型エイリアスです。
        using difference_type = ISize;

        /// 要素型のポインタ型エイリアスです。
        using pointer = const T*;
        
        /// 要素型の参照型エイリアスです。
        using reference = const T&;

        /// 初期化します。
        /// @param pointer メモリ上の現在地を指すポインタです。
        ConstPointerIterator(T *pointer) noexcept
        : m_element(pointer)
        {}

        /// コピーします。
        /// @param origin コピー元です。
        ConstPointerIterator(const ConstPointerIterator<T> &origin) noexcept
        : m_element(origin.m_element)
        {}

        /// ムーブします。
        /// @param origin ムーブ元です。
        ConstPointerIterator(ConstPointerIterator<T> &&origin) noexcept
        : m_element(origin.m_element)
        {
            origin.m_element = nullptr;
        }

        /// コピー代入します。
        /// @param origin コピー元です。
        ConstPointerIterator<T>& operator=(const ConstPointerIterator<T> &origin) noexcept
        {
            this->m_element = origin.m_element;
            return *this;
        }

        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        ConstPointerIterator<T>& operator=(ConstPointerIterator<T> &&origin) noexcept
        {
            this->m_element = origin.m_element;
            origin.m_element = nullptr;
            return *this;
        }

        /// 指定分進めます。
        /// @param step 進める数です。
        ConstPointerIterator<T>& operator+=(USize step) noexcept
        {
            this->m_element += step;
            return *this;
        }

        /// 指定分戻ります。
        /// @param step 戻る数です。
        ConstPointerIterator<T>& operator-=(USize step) noexcept
        {
            this->m_element -= step;
            return *this;
        }
        
        /// 一つ進めます。
        ConstPointerIterator<T>& operator++() noexcept
        {
            this->m_element += 1;
            return *this;
        }
        
        /// 一つ進めて、進む前のイテレータを返します。
        /// @return 進める前のイテレータです。
        ConstPointerIterator<T> operator++(int) noexcept
        {
            auto iter = *this;
            this->m_element += 1;
            return iter;
        }
        
        /// 一つ戻ります。
        ConstPointerIterator<T>& operator--() noexcept
        {
            this->m_element -= 1;
            return *this;
        }
        
        /// 一つ戻って、戻る前のイテレータを返します。
        /// @return 戻る前のイテレータです。
        ConstPointerIterator<T> operator--(int) noexcept
        {
            auto iter = *this;
            this->m_element -= 1;
            return iter;
        }

        /// 要素にアクセスします。
        /// @exception NullRefarenceException 要素がnullptrの可能性があります。
        const T &operator*() const;

        /// 要素が同等か比較します。
        /// @param other 比較対象です。
        /// @return 同等の場合、真です。
        Bool operator==(const ConstPointerIterator<T> &other) const noexcept
        {
            return l.m_element == r.m_element;
        }

        /// 要素が不等か比較します。
        /// @param other 比較対象です。
        /// @return 不等の場合、真です。
        Bool operator!=(const ConstPointerIterator<T> &other) const noexcept
        {
            return l.m_element != r.m_element;
        }
    };

    /// 標準文字列型です。
    DYNAMIC_LINK_CORE class String
    {
        USize m_length; // 文字列の長さです。
        USize m_hash;   // 文字列のハッシュ値です。
        Char *m_string; // 中央管理された文字列へのポインタです。

    public:

        /// 空の文字列で初期化します。
        String() noexcept;

        /// 文字列を指定して初期化します。
        /// @param string 指定の文字列です。 @n nullptrを指定した場合、空の文字列として扱います。
        /// @exception BufferOverException 指定された文字列がUSizeで表現可能な数値を超える可能性があります。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        String(const Char *string);

        /// コピーします。
        /// @param origin コピー元です。
        String(const String &origin) noexcept;
        
        /// ムーブします。
        /// @param origin ムーブ元です。
        String(String &&origin) noexcept;

        /// 解体します。
        ~String() noexcept;

        /// コピー代入します。
        /// @param origin コピー元です。
        String& operator=(const String &origin) noexcept;
        
        /// ムーブ代入します。
        /// @param origin ムーブ元です。
        String& operator=(String &&origin) noexcept;

        /// 結合代入します。
        /// @param r 結合元です。
        /// @exception BufferOverException 結合した文字列がUSizeで表現可能な数値を超える可能性があります。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        String& operator+=(const String &r);

        /// 添え字から要素を取得します。
        /// @param index 取得する位置です。
        /// @exception OutOfRangeException 添え字が文字列の範囲外の可能性があります。
        Char operator[](USize index) const;

        /// 結合代入します。
        /// @param r 結合元です。
        /// @exception BufferOverException 結合した文字列がUSizeで表現可能な数値を超える可能性があります。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        String& add(const String &r);

        /// 一部を抜き出します。
        /// @param startIndex 抜き出し始める位置です。
        /// @param length 抜き出す文字列の長さです。
        /// @exception OutOfRangeException 抜き出し始め位置や、そこから抜き出す長さが文字列の範囲外の可能性があります。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        String sub(USize startIndex, USize length) const;

        /// 添え字から要素を取得します。
        /// @param index 取得する位置です。
        /// @exception OutOfRangeException 添え字が文字列の範囲外の可能性があります。
        Char at(USize index) const;

        /// 文字列の長さを取得します。
        /// @return 文字列の長さです。 @n UTF-8の為、文字数ではないことに注意してください。
        USize length() const noexcept;

        /// 生文字列を取得します。
        /// @return 生文字列です。
        const Char *raw() const noexcept;

        /// 先頭の不変イテレータを返します。
        /// @return 先頭の可変イテレータです。
        ConstPointerIterator<Char> begin() const noexcept;

        /// 番兵の不変イテレータを返します。
        /// @return 番兵の可変イテレータです。
        ConstPointerIterator<Char> end() const noexcept;

        /// 結合します。
        /// @param r 結合元です。
        /// @exception BufferOverException 指定された文字列がUSizeで表現可能な数値を超える可能性があります。
        /// @exception MemoryException メモリの確保に失敗する可能性があります。
        String operator+(const String &r) const;

        /// 文字列が等しいか比較します。
        /// @param r 比較対象の文字列です。
        /// @return 等しい場合、真です。
        Bool operator==(const String &r) const noexcept;

        /// 文字列が等しいか比較します。
        /// @param r 比較対象の文字列です。
        /// @return 等しい場合、真です。
        Bool operator!=(const String &r) const noexcept;
    };

    /// 参照がnullptrの場合の例外です。
    DYNAMIC_LINK_CORE class NullReferenceException: public IException
    {
        String m_message;

    public:

        /// 初期化します。
        /// @param message 
        NullReferenceException(const String &message) noexcept;

        /// コピーします。
        /// @param origine コピー元です。
        NullReferenceException(const NullReferenceException &origin) noexcept;

        /// ムーブします。
        /// @param origine ムーブ元です。
        NullReferenceException(NullReferenceException &&origin) noexcept;

        /// コピー代入します。
        /// @param origine コピー元です。
        NullReferenceException& operator=(const NullReferenceException &origin) noexcept;

        /// ムーブ代入します。
        /// @param origine ムーブ元です。
        NullReferenceException& operator=(NullReferenceException &&origin) noexcept;

        /// エラーメッセージを取得します。
        /// @return エラーメッセージです。
        const Char *message() const noexcept override;
    };

    /// 要素にアクセスします。
    /// @exception NullRefarenceException 要素がnullptrの可能性があります。
    template<typename T>
    T &PointerIterator<T>::operator*()
    {
        if (!this->m_element) throw NullReferenceException(TXT("指定ポインタがnullptrでした。"));

        return *this->m_element;
    }

    /// 要素にアクセスします。
    /// @exception NullRefarenceException 要素がnullptrの可能性があります。
    template<typename T>
    const T &PointerIterator<T>::operator*() const
    {
        if (!this->m_element) throw NullReferenceException(TXT("指定ポインタがnullptrでした。"));

        return *this->m_element;
    }

    /// 要素にアクセスします。
    /// @exception NullRefarenceException 要素がnullptrの可能性があります。
    template<typename T>
    const T &ConstPointerIterator<T>::operator*() const
    {
        if (!this->m_element) throw NullReferenceException(TXT("指定ポインタがnullptrでした。"));

        return *this->m_element;
    }
}

#endif // !_FURAIENGINE_STRING_HPP