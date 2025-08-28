#pragma once

#include <cmath>       // sqrtfなどの数学関数
#include <type_traits> // is_floating_point_vなどの型情報


// テンプレートベクトル構造体 TVector2<T>
template <typename T>
struct TVector2
{
    T x, y;

    // --- コンストラクタ ---

    // デフォルトコンストラクタ
    constexpr TVector2(T _x = 0, T _y = 0) noexcept : x(_x), y(_y) {}

    // 異なる型のTVector2からの変換コンストラクタ
    // NOTE: explicitを付け、意図しない暗黙の型変換を防ぐ
    template <typename U>
    explicit constexpr TVector2(const TVector2<U>& other) noexcept
        : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {
    }

    // --- 単項演算子 ---
    constexpr TVector2<T> operator+() const noexcept { return *this; }
    constexpr TVector2<T> operator-() const noexcept { return TVector2<T>(-x, -y); }

    // --- 複合代入演算子 ---
    TVector2<T>& operator+=(const TVector2<T>& other) noexcept { x += other.x; y += other.y; return *this; }
    TVector2<T>& operator-=(const TVector2<T>& other) noexcept { x -= other.x; y -= other.y; return *this; }
    TVector2<T>& operator*=(T scalar) noexcept { x *= scalar; y *= scalar; return *this; }
    TVector2<T>& operator/=(T scalar) noexcept { x /= scalar; y /= scalar; return *this; }

    // --- 比較演算子 ---
    constexpr bool operator==(const TVector2<T>& other) const noexcept { return (x == other.x) && (y == other.y); }
    constexpr bool operator!=(const TVector2<T>& other) const noexcept { return !(*this == other); }

    // --- 便利関数 ---

    // 長さの2乗を返す
    T LengthSq() const noexcept
    {
        return x * x + y * y;
    }

    // 長さを返す (戻り値はfloatで統一すると扱いやすい)
    float Length() const noexcept
    {
        return std::sqrtf(static_cast<float>(LengthSq()));
    }

    // ドット積（内積）
    T Dot(const TVector2<T>& other) const noexcept
    {
        return x * other.x + y * other.y;
    }

    // 2D外積
    T Cross(const TVector2<T>& other) const noexcept
    {
        return x * other.y - y * other.x;
    }

    // --- 型に特化した便利関数 (C++17: if constexpr) ---

    // ベクトルを正規化する（長さを1にする）
    // NOTE: 浮動小数点型(float, double)の場合のみ有効な実装
    void Normalize() noexcept
    {
        if constexpr (std::is_floating_point_v<T>)
        {
            float len = Length();
            constexpr T epsilon = static_cast<T>(1e-6);
            if (len > epsilon)
            {
                T invLen = static_cast<T>(1.0) / len;
                x *= invLen;
                y *= invLen;
            }
        }
        // 整数型などでこの関数が呼ばれても、何もしない（コンパイルエラーにもならない）
    }

    // 正規化されたベクトルを新しいインスタンスとして返す
    TVector2<T> GetNormalized() const noexcept
    {
        TVector2<T> temp = *this;
        temp.Normalize();
        return temp;
    }
};

// --- 二項演算子 (非メンバ関数) ---
// NOTE: テンプレートクラス内ではなく、外で定義することで、様々な型の左辺値に対応できる（例：int * TVector2<float>など）
//       今回はシンプルにするため、T型のスカラーのみを想定

template <typename T>
constexpr TVector2<T> operator+(const TVector2<T>& lhs, const TVector2<T>& rhs) noexcept { return TVector2<T>(lhs.x + rhs.x, lhs.y + rhs.y); }

template <typename T>
constexpr TVector2<T> operator-(const TVector2<T>& lhs, const TVector2<T>& rhs) noexcept { return TVector2<T>(lhs.x - rhs.x, lhs.y - rhs.y); }

template <typename T>
constexpr TVector2<T> operator*(const TVector2<T>& vec, T scalar) noexcept { return TVector2<T>(vec.x * scalar, vec.y * scalar); }

template <typename T>
constexpr TVector2<T> operator*(T scalar, const TVector2<T>& vec) noexcept { return TVector2<T>(vec.x * scalar, vec.y * scalar); }

template <typename T>
constexpr TVector2<T> operator/(const TVector2<T>& vec, T scalar) noexcept { return TVector2<T>(vec.x / scalar, vec.y / scalar); }



// 型エイリアス
using Vector2F = TVector2<float>;
using Vector2I = TVector2<int>;