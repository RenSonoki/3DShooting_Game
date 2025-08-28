#include "PlayerEntity.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include "BulletShooterComponent.h"

PlayerEntity::PlayerEntity()
{
    SetTag(L"Player");
}

// ゲッターの実装は変更なし
std::shared_ptr<TransformComponent> PlayerEntity::GetTransform() const { return GetComponent<TransformComponent>(); }
std::shared_ptr<HealthComponent> PlayerEntity::GetHealth() const { return GetComponent<HealthComponent>(); }
std::shared_ptr<BulletShooterComponent> PlayerEntity::GetShooter() const { return GetComponent<BulletShooterComponent>(); }

int PlayerEntity::GetHP() const
{
    if (auto health = GetHealth()) return health->GetHP();
    return 0;
}

bool PlayerEntity::IsAlive() const
{
    if (auto health = GetHealth()) return health->IsAlive();
    return false;
}

bool PlayerEntity::IsDead() const
{
    // 自身のHealthComponentを取得
    if (auto health = GetComponent<HealthComponent>())
    {
        // HealthComponentのIsDead()の結果を返す
        return health->IsDead();
    }
    // HealthComponentがなければ、安全のために「死亡している」とみなす
    return true;
}