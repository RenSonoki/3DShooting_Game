#include "ButtonRenderer.h"
#include "UIButton.h" // オーナーをUIButtonにキャストして状態を取得するため

ButtonRenderer::ButtonRenderer(const std::wstring& normalPath,
    const std::wstring& hoverPath,
    const std::wstring& pressedPath)
{
    m_rendererNormal.Load(normalPath);
    m_rendererHover.Load(hoverPath.empty() ? normalPath : hoverPath);
    m_rendererPressed.Load(pressedPath.empty() ? normalPath : pressedPath);
}

void ButtonRenderer::Draw(const UIElement* owner) const
{
    const auto* button = static_cast<const UIButton*>(owner);
    if (!button) return;

    const SpriteRenderer* rendererToUse = &m_rendererNormal;
    switch (button->GetState())
    {
    case UIButton::ButtonState::Hovered:
        rendererToUse = &m_rendererHover;
        break;
    case UIButton::ButtonState::Pressed:
        rendererToUse = &m_rendererPressed;
        break;
    default:
        break;
    }

    rendererToUse->Draw(button->GetTransform());
}

VECTOR2 ButtonRenderer::GetNormalSpriteSize() const
{
    return {
        (float)m_rendererNormal.GetOriginalWidth(),
        (float)m_rendererNormal.GetOriginalHeight()
    };
}