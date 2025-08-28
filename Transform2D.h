#pragma once
#include <cmath>
#include <algorithm> // std::clampなど、他の場所で必要になる可能性

// 2Dベクトル構造体
struct VECTOR2
{
    float x = 0.0f;
    float y = 0.0f;

    // コンストラクタ
    VECTOR2() = default;
    constexpr VECTOR2(float x, float y) : x(x), y(y) {}

    // --- 複合代入演算子 ---
    VECTOR2& operator+=(const VECTOR2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    VECTOR2& operator-=(const VECTOR2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    VECTOR2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    VECTOR2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    // --- 便利関数 ---
    float LengthSq() const { return x * x + y * y; }
    float Length() const { return std::sqrtf(LengthSq()); }
    VECTOR2 Normalized() const
    {
        float len = Length();
        if (len > 1e-6f)
        { // ゼロ除算を防止
            return { x / len, y / len };
        }
        return { 0.0f, 0.0f };
    }

    // 指定した角度（ラジアン）でこのベクトルを回転させた新しいベクトルを返す
    VECTOR2 RotatedBy(float angleRad) const
    {
        float sinRot = sinf(angleRad);
        float cosRot = cosf(angleRad);
        return {
            x * cosRot - y * sinRot,
            x * sinRot + y * cosRot
        };
    }
};

// --- VECTOR2のためのグローバルな演算子 ---
inline constexpr VECTOR2 operator+(const VECTOR2& lhs, const VECTOR2& rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
inline constexpr VECTOR2 operator-(const VECTOR2& lhs, const VECTOR2& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }
inline constexpr VECTOR2 operator*(const VECTOR2& vec, float scalar) { return { vec.x * scalar, vec.y * scalar }; }
inline constexpr VECTOR2 operator*(float scalar, const VECTOR2& vec) { return { vec.x * scalar, vec.y * scalar }; }
inline constexpr VECTOR2 operator/(const VECTOR2& vec, float scalar) { return { vec.x / scalar, vec.y / scalar }; }
// 成分ごとの乗算
inline constexpr VECTOR2 operator*(const VECTOR2& a, const VECTOR2& b) { return { a.x * b.x, a.y * b.y }; }
inline constexpr bool operator==(const VECTOR2& lhs, const VECTOR2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
inline constexpr bool operator!=(const VECTOR2& lhs, const VECTOR2& rhs) { return !(lhs == rhs); }

// -------------------------------------------------------------------------------------------------------------------------------

// UIや2Dオブジェクト用の空間情報クラス
class Transform2D
{
public:
    Transform2D();

    // 位置
    void SetPosition(const VECTOR2& pos) { m_position = pos; }
    VECTOR2 GetPosition() const { return m_position; }

    // スケール
    void SetScale(const VECTOR2& scale) { m_scale = scale; }
    VECTOR2 GetScale() const { return m_scale; }

    // 回転（ラジアン）
    void SetRotation(float rot) { m_rotation = rot; }
    float GetRotation() const { return m_rotation; }

    // 回転・拡縮の中心点（0.0-1.0の範囲）
    void SetPivot(const VECTOR2& pivot) { m_pivot = pivot; }
    VECTOR2 GetPivot() const { return m_pivot; }

    // 状態を初期値に戻す
    void Reset();

    // 指定したワールド座標の点が、このTransformを持つ矩形(size)の範囲内にあるか判定
    bool Contains(const VECTOR2& worldPoint, const VECTOR2& size) const;

private:
    VECTOR2 m_position = { 0.0f, 0.0f };
    VECTOR2 m_scale = { 1.0f, 1.0f };
    float   m_rotation = 0.0f;
    VECTOR2 m_pivot = { 0.5f, 0.5f }; // 中心
};

// --- Transform2D の実装 ---

inline Transform2D::Transform2D()
{
    // デフォルトコンストラクタはクラス定義内で初期化済み
}

inline void Transform2D::Reset()
{
    m_position = { 0.0f, 0.0f };
    m_scale = { 1.0f, 1.0f };
    m_rotation = 0.0f;
    m_pivot = { 0.5f, 0.5f };
}

inline bool Transform2D::Contains(const VECTOR2& worldPoint, const VECTOR2& size) const
{
    if (m_scale.x <= 0.0f || m_scale.y <= 0.0f) {
        return false;
    }

    // Step 1: 逆平行移動
    VECTOR2 point = worldPoint - m_position;

    // Step 2: 逆回転 (ヘルパーメソッドを使って簡潔に)
    if (m_rotation != 0.0f) {
        point = point.RotatedBy(-m_rotation); // ここがスッキリ！
    }

    // Step 3: 逆スケール
    point.x /= m_scale.x;
    point.y /= m_scale.y;

    // Step 4: ピボットを考慮して判定
    VECTOR2 halfSize = { size.x / 2.0f, size.y / 2.0f };
    VECTOR2 pivotOffset = { (m_pivot.x - 0.5f) * size.x, (m_pivot.y - 0.5f) * size.y };
    point -= pivotOffset;

    return (point.x >= -halfSize.x && point.x <= halfSize.x &&
        point.y >= -halfSize.y && point.y <= halfSize.y);
}