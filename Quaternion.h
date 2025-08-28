#pragma once
#include <cmath>
#include <DxLib.h>

// DXライブラリに依存しない、自前のQuaternionクラス
struct Quaternion
{
    float x, y, z, w;

    // --- コンストラクタ ---
    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // --- 静的な生成関数 ---

    // 単位クォータニオン（無回転）を返す
    static Quaternion Identity() { return Quaternion(0, 0, 0, 1); }

    // 新しく追加：回転行列からクォータニオンを生成する静的メソッド
    static Quaternion FromMatrix(const MATRIX& m)
    {
        Quaternion q;
        float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];
        if (trace > 0.0f) {
            float s = 0.5f / sqrtf(trace + 1.0f);
            q.w = 0.25f / s;
            q.x = (m.m[2][1] - m.m[1][2]) * s;
            q.y = (m.m[0][2] - m.m[2][0]) * s;
            q.z = (m.m[1][0] - m.m[0][1]) * s;
        }
        else {
            if (m.m[0][0] > m.m[1][1] && m.m[0][0] > m.m[2][2]) {
                float s = 2.0f * sqrtf(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]);
                q.w = (m.m[2][1] - m.m[1][2]) / s;
                q.x = 0.25f * s;
                q.y = (m.m[0][1] + m.m[1][0]) / s;
                q.z = (m.m[0][2] + m.m[2][0]) / s;
            }
            else if (m.m[1][1] > m.m[2][2]) {
                float s = 2.0f * sqrtf(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]);
                q.w = (m.m[0][2] - m.m[2][0]) / s;
                q.x = (m.m[0][1] + m.m[1][0]) / s;
                q.y = 0.25f * s;
                q.z = (m.m[1][2] + m.m[2][1]) / s;
            }
            else {
                float s = 2.0f * sqrtf(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]);
                q.w = (m.m[1][0] - m.m[0][1]) / s;
                q.x = (m.m[0][2] + m.m[2][0]) / s;
                q.y = (m.m[1][2] + m.m[2][1]) / s;
                q.z = 0.25f * s;
            }
        }
        return q;
    }

    // オイラー角（ラジアン）からクォータニオンを生成
    static Quaternion FromEulerAngles(float pitch, float yaw, float roll)
    {
        float cy = cosf(yaw * 0.5f);
        float sy = sinf(yaw * 0.5f);
        float cp = cosf(pitch * 0.5f);
        float sp = sinf(pitch * 0.5f);
        float cr = cosf(roll * 0.5f);
        float sr = sinf(roll * 0.5f);

        Quaternion q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;
        return q;
    }

    // --- 演算子オーバーロード ---

    // クォータニオンの積（回転の合成）
    Quaternion operator*(const Quaternion& rhs) const
    {
        Quaternion q;
        q.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
        q.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
        q.y = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
        q.z = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;
        return q;
    }

    // --- 便利関数 ---

    // 正規化
    void Normalize()
    {
        float length = sqrtf(x * x + y * y + z * z + w * w);
        if (length > 1e-6f)
        {
            x /= length; y /= length; z /= length; w /= length;
        }
    }

    // クォータニオンから回転行列を生成
    MATRIX ToMatrix() const
    {
        MATRIX m;
        m.m[0][0] = 1.0f - 2.0f * (y * y + z * z);
        m.m[0][1] = 2.0f * (x * y + w * z);
        m.m[0][2] = 2.0f * (x * z - w * y);
        m.m[0][3] = 0.0f;

        m.m[1][0] = 2.0f * (x * y - w * z);
        m.m[1][1] = 1.0f - 2.0f * (x * x + z * z);
        m.m[1][2] = 2.0f * (y * z + w * x);
        m.m[1][3] = 0.0f;

        m.m[2][0] = 2.0f * (x * z + w * y);
        m.m[2][1] = 2.0f * (y * z - w * x);
        m.m[2][2] = 1.0f - 2.0f * (x * x + y * y);
        m.m[2][3] = 0.0f;

        m.m[3][0] = 0.0f;
        m.m[3][1] = 0.0f;
        m.m[3][2] = 0.0f;
        m.m[3][3] = 1.0f;
        return m;
    }

    // 球面線形補間 (Slerp)
    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t)
    {
        float dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

        Quaternion tempB = b;
        if (dot < 0.0f)
        {
            dot = -dot;
            tempB.x = -b.x; tempB.y = -b.y; tempB.z = -b.z; tempB.w = -b.w;
        }

        if (dot > 0.9995f)
        {
            // ほぼ同じ向きなら線形補間
            Quaternion result = Quaternion(
                a.x + t * (tempB.x - a.x),
                a.y + t * (tempB.y - a.y),
                a.z + t * (tempB.z - a.z),
                a.w + t * (tempB.w - a.w)
            );
            result.Normalize();
            return result;
        }

        float theta_0 = acosf(dot);
        float theta = theta_0 * t;
        float sin_theta = sinf(theta);
        float sin_theta_0 = sinf(theta_0);

        float s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;

        return Quaternion(
            (s0 * a.x) + (s1 * tempB.x),
            (s0 * a.y) + (s1 * tempB.y),
            (s0 * a.z) + (s1 * tempB.z),
            (s0 * a.w) + (s1 * tempB.w)
        );
    }
};