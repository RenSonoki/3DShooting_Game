#include "MovementSystem.h"
#include "Entity.h"
#include "TransformComponent.h" // Transformを直接操作するためインクルード
#include <DxLib.h>

void MovementSystem::Update(const std::vector<std::shared_ptr<Entity>>& entities, float deltaTime)
{
    // 管理されている全てのエンティティをループ
    for (const auto& entity : entities)
    {
        // エンティティがアクティブで、TransformComponentを持っているかチェック
        if (entity && entity->IsActive())
        {
            if (auto transform = entity->GetComponent<TransformComponent>())
            {
                // 速度を取得
                const VECTOR& vel = transform->GetVelocity();

                // 速度が0でなければ、位置を更新
                if (VSquareSize(vel) > 1e-9f)
                {
                    VECTOR pos = transform->GetPosition();
                    pos = VAdd(pos, VScale(vel, deltaTime));
                    transform->SetPosition(pos);
                }
            }
        }
    }
}