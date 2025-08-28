#pragma once
#include "ComponentBase.h" // ★ 基底クラスを変更
#include <memory>
#include <functional>

// 前方宣言
class TransformComponent;
class EntitySystem;
class BulletPrototype;
class Entity;

/**
 * @class BulletShooterComponent
 * @brief 弾の発射とクールダウンを制御するコンポーネント
 */
class BulletShooterComponent : public ComponentBase
{
public:
    // ★ 修正点: デフォルトコンストラクタに変更
    BulletShooterComponent();
    virtual ~BulletShooterComponent() = default;

    ComponentID GetID() const override;

    void Start() override;
    void Update(float deltaTime) override;

    /**
     * @brief このシューターが使用する設計図と、弾を追加するシステムを設定します
     * @param prototype 弾の設計図
     * @param entitySystem 弾の追加先となるシステム
     */
    void Setup(std::shared_ptr<BulletPrototype> prototype, EntitySystem* entitySystem);

    // 発射リクエスト
    void RequestShoot();

    // クールダウン時間を秒数で設定
    void SetCooldown(float seconds);

    // 現在発射可能か（クールダウンが終わっているか）
    bool CanShoot() const;

private:
    void Shoot(); // 内部的な発射処理

    std::shared_ptr<TransformComponent> m_transform;
    EntitySystem* m_entitySystem;
    std::shared_ptr<BulletPrototype> m_prototype;

    float m_cooldownTime;
    float m_timer;
    bool m_isShootRequested;
};