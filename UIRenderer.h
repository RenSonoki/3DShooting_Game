#pragma once
class UIElement;

// 「どのように描画するか」という振る舞いを定義するインターフェース
class UIRenderer
{
public:
    virtual ~UIRenderer() = default;
    virtual void Draw(const UIElement* owner) const = 0;
};