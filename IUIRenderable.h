#pragma once

// UI描画用インターフェース
class IUIRenderable
{
public:
    virtual ~IUIRenderable() = default;
    virtual void Draw() const = 0; // const に統一
};