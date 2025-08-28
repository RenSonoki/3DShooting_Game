#pragma once

// UIのロジックを更新するためのインターフェース
class IUILogicUpdatable
{
public:
    virtual ~IUILogicUpdatable() = default;
    virtual void UpdateLogic(float deltaTime) = 0;
};
