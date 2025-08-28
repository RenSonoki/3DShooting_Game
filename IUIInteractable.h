#pragma once

// UIのインタラクションを管理するインターフェース
class IUIInteractable
{
public:
    virtual ~IUIInteractable() = default;
    virtual void UpdateInteraction() = 0;
    virtual void OnClick() = 0;
    virtual void OnHover() {}
};