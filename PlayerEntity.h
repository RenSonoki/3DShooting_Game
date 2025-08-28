#pragma once
#include "Entity.h"
#include <memory>

// 前方宣言
class TransformComponent;
class HealthComponent;
class BulletShooterComponent;

class PlayerEntity : public Entity
{
public:
    PlayerEntity();

    // 便利なゲッターはそのまま残します
    std::shared_ptr<TransformComponent> GetTransform() const;
    std::shared_ptr<HealthComponent> GetHealth() const;
    std::shared_ptr<BulletShooterComponent> GetShooter() const;

    int GetHP() const;
    bool IsAlive() const;
    bool IsDead() const;
};