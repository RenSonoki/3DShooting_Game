#pragma once // 修正済み
#include "ComponentBase.h"

// 描画を担うコンポーネントの抽象基底クラス
class DrawComponent : public ComponentBase
{
public:
    virtual ~DrawComponent() = default;

    // DrawComponentは必ずDrawを持つべき、というルールを強制する
    virtual void Draw() = 0;
};