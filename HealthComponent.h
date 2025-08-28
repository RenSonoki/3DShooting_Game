#pragma once
#include "ComponentBase.h"
#include <functional>

/**
 * @class HealthComponent
 * @brief キャラクターの体力や生死、無敵状態を管理するコンポーネント
 */
class HealthComponent : public ComponentBase
{
public:
    HealthComponent();

    ComponentID GetID() const override;

    // --- ライフサイクルメソッド ---
    void Update(float deltaTime) override;

    /**
     * @brief コンポーネントの初期設定を行います。
     * @param maxHP 最大体力
     * @param initialInvincibilitySec 生成直後の無敵時間（秒）
     */
    void Setup(int maxHP, float initialInvincibilitySec = 0.0f);

    // --- アクション ---
    void TakeDamage(int amount);
    void Heal(int amount);
    void SetInvincible(float durationSec);

    // --- 状態取得ゲッター ---
    int GetHP() const;
    int GetMaxHP() const;
    bool IsAlive() const;
    bool IsDead() const;
    bool IsInvincible() const;
    float GetInvincibleTimeRemaining() const;

    // --- イベントコールバック設定 ---
    void SetOnDeathCallback(const std::function<void()>& callback);
    void SetOnDamageCallback(const std::function<void(int)>& callback);
    void SetOnHealCallback(const std::function<void(int)>& callback);

private:
    int m_maxHP;
    int m_currentHP;
    bool m_dead;
    float m_invincibleTimer;

    // 各種コールバック関数
    std::function<void()> m_onDeathCallback;
    std::function<void(int)> m_onDamageCallback;
    std::function<void(int)> m_onHealCallback;
};