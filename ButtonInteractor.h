#pragma once
#include "UIInteractor.h"

// ボタンの「インタラクション」振る舞いを担当するクラス
class ButtonInteractor : public UIInteractor
{
public:
    // UIInteractorインターフェースの実装
    void UpdateInteraction(UIElement* owner) override;

private:
    bool m_previousMouseDown = false;
};