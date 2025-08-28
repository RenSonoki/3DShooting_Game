#pragma once
#include "UIRenderer.h"
#include "SpriteRenderer.h"
#include <string>

// ボタンの「描画」振る舞いを担当するクラス
class ButtonRenderer : public UIRenderer
{
public:
    ButtonRenderer(const std::wstring& normalPath,
        const std::wstring& hoverPath,
        const std::wstring& pressedPath);

    // UIRendererインターフェースの実装
    void Draw(const UIElement* owner) const override;

    // UIButtonが自身のサイズを知るために、通常画像のサイズを返す
    VECTOR2 GetNormalSpriteSize() const;

private:
    SpriteRenderer m_rendererNormal;
    SpriteRenderer m_rendererHover;
    SpriteRenderer m_rendererPressed;
};