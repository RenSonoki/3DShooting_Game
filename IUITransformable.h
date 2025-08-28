#pragma once
#include "Transform2D.h"

// Transform情報を持つUI用インターフェース
class IUITransformable
{
public:
    virtual ~IUITransformable() = default;
    virtual Transform2D& GetTransform() = 0;
    virtual const Transform2D& GetTransform() const = 0;
};