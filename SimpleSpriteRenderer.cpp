#include "SimpleSpriteRenderer.h"
#include "UIElement.h" // オーナーのTransformなどを取得するために必要

SimpleSpriteRenderer::SimpleSpriteRenderer(const std::wstring& imagePath)
{
    // コンストラクタで、内包するSpriteRendererに画像をロードさせる
    m_spriteRenderer.Load(imagePath);
}

void SimpleSpriteRenderer::Draw(const UIElement* owner) const
{
    if (!owner) return;

    // オーナーからTransform情報を取得し、内包するSpriteRendererに描画を委譲
    m_spriteRenderer.Draw(owner->GetTransform());
}