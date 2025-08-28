#pragma once
#include "ComponentBase.h"

// ゲームロジックを担うコンポーネントの抽象基底クラス
class LogicComponent : public ComponentBase
{
public:
    virtual ~LogicComponent() = default;

    // LogicComponentは必ずUpdateを持つべき、というルールを強制する
    // BaseクラスのUpdateをオーバーライドし、純粋仮想にする
    void Update(float deltaTime) override = 0;
};