#pragma once
#include <string>
#include "Transform2D.h"
#include "Color.h"

namespace BlendMode
{
    constexpr int None = 0;
    constexpr int Alpha = 2;
    constexpr int Add = 1;
    constexpr int Subtract = 3;
}

/**
 * @class SpriteRenderer
 * @brief 一枚のスプライト画像の描画に必要なデータとロジックをカプセル化するクラス
 */
class SpriteRenderer
{
public:
    SpriteRenderer();
    explicit SpriteRenderer(const std::wstring& path);
    ~SpriteRenderer();

    // コピーは禁止し、ムーブ（所有権の移動）を許可する
    SpriteRenderer(const SpriteRenderer&) = delete;
    SpriteRenderer& operator=(const SpriteRenderer&) = delete;
    SpriteRenderer(SpriteRenderer&& other) noexcept;
    SpriteRenderer& operator=(SpriteRenderer&& other) noexcept;

    bool Load(const std::wstring& path);

    // 描画メソッド（引数はTransformのみでシンプルに）
    void Draw(const Transform2D& transform) const;

    // --- 描画パラメータを設定するセッター ---
    void SetVisible(bool visible);
    void SetColor(unsigned int color);
    void SetBlendParam(int blendParam); // ブレンドの強さを設定

    // --- ゲッター ---
    int GetOriginalWidth() const;
    int GetOriginalHeight() const;
    int GetHandle() const;

private:
    void Release(); // 解放処理をまとめるヘルパー

    int m_handle;
    int m_originalWidth;
    int m_originalHeight;

    bool m_visible;
    unsigned int m_color;
    int m_blendParam; // ブレンドの強さ(0-255)
};