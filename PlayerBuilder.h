#pragma once
#include <string>
#include <memory>

// 前方宣言
class PlayerEntity;
class EntitySystem;
class BulletPrototype;

class PlayerBuilder
{
public:
    PlayerBuilder();

    // メソッドチェーンで設定を繋げられるように、自身の参照を返す
    PlayerBuilder& SetModel(const std::wstring& modelPath);
    PlayerBuilder& SetShooterInfo(std::shared_ptr<BulletPrototype> prototype, EntitySystem* entitySystem);
    PlayerBuilder& SetInitialHealth(int maxHP, float invincibilitySec);
    PlayerBuilder& SetCollisionRadius(float radius);

    // 最終的に設定を元にPlayerEntityを構築して返す
    std::shared_ptr<PlayerEntity> Build() const;

private:
    // 構築に必要なパラメータを保持
    std::wstring m_modelPath;
    std::shared_ptr<BulletPrototype> m_bulletPrototype;
    EntitySystem* m_entitySystem;
    int m_maxHP;
    float m_initialInvincibility;
    float m_collisionRadius;
};