#pragma once

// 表示状態管理インターフェース
class IUIVisible
{
public:
    virtual ~IUIVisible() = default;
    virtual void SetVisible(bool visible) = 0;
    virtual bool IsVisible() const = 0;
};
