#pragma once
#include "UIRenderer.h"
#include "SpriteRenderer.h"
#include <string>

// 単純なスプライト描画の「振る舞い」をカプセル化したクラス
class SimpleSpriteRenderer : public UIRenderer
{
public:
    explicit SimpleSpriteRenderer(const std::wstring& imagePath);

    // UIRendererインターフェースを実装
    void Draw(const UIElement* owner) const override;

private:
    // 実際にDXライブラリの描画を行うクラスを内包
    SpriteRenderer m_spriteRenderer;
};