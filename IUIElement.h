#pragma once
#include "IUIVisible.h"
#include "IUIRenderable.h"
#include "IUILogicUpdatable.h"
#include "IUITransformable.h"

class IUIElement :
    public IUIVisible,
    public IUIRenderable,
    public IUILogicUpdatable,
    public IUITransformable
{
public:
    virtual ~IUIElement() = default;
};