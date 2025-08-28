#include "ButtonInteractor.h"
#include "UIButton.h"
#include "VirtualScreenManager.h"
#include "DxLib.h"

void ButtonInteractor::UpdateInteraction(UIElement* owner)
{
    auto* button = static_cast<UIButton*>(owner);
    if (!button || !button->IsVisible())
    {
        if (button) button->SetState(UIButton::ButtonState::Normal);
        return;
    }

    // 当たり判定ロジック
    const Vector2I mousePosInt = VirtualScreenManager::GetInstance().ConvertMousePositionToVirtual();
    const VECTOR2 mousePos = { (float)mousePosInt.x, (float)mousePosInt.y };
    const VECTOR2 size = owner->GetBoundingSize(); // オーナーにサイズを聞くだけ
    const bool isOver = owner->GetTransform().Contains(mousePos, size);

    // 状態遷移とイベント発火ロジック
    const bool isDown = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

    if (isOver)
    {
        button->SetState(isDown ? UIButton::ButtonState::Pressed : UIButton::ButtonState::Hovered);
        if (m_previousMouseDown && !isDown)
        {
            button->InvokeOnClick();
        }
    }
    else
    {
        button->SetState(UIButton::ButtonState::Normal);
    }

    m_previousMouseDown = isDown;
}