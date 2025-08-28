#pragma once
class UIElement;

// 「ユーザー操作にどう反応するか」という振る舞いを定義するインターフェース
class UIInteractor
{
public:
    virtual ~UIInteractor() = default;
    virtual void UpdateInteraction(UIElement* owner) = 0;
};

