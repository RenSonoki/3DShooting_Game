#pragma once
#include "UIRenderer.h"

// テキストの「描画」振る舞いをカプセル化したクラス
class TextRenderer : public UIRenderer
{
public:
    // UIRendererインターフェースを実装
    void Draw(const UIElement* owner) const override;
};