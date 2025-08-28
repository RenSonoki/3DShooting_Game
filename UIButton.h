#pragma once
#include "UIElement.h"
#include <functional>
#include <string>

// 振る舞いを外部部品に委譲する、データ中心のボタンクラス
class UIButton : public UIElement
{
public:
    enum class ButtonState { Normal, Hovered, Pressed };

    UIButton(const std::wstring& normalPath,
        const std::wstring& hoverPath = L"",
        const std::wstring& pressedPath = L"");

    // --- 外部から使うためのインターフェース ---
    void SetOnClick(std::function<void()> callback);

    // --- システム内部（部品）から使われるメソッド ---
    void InvokeOnClick();
    ButtonState GetState() const;
    void SetState(ButtonState newState);
    VECTOR2 GetBoundingSize() const override;

private:
    ButtonState m_state = ButtonState::Normal;
    std::function<void()> m_onClick;
};