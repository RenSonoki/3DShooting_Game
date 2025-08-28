#include "HealthComponent.h"
#include <algorithm> // std::max のため

HealthComponent::HealthComponent()
    : m_maxHP(100)
    , m_currentHP(100)
    , m_dead(false)
    , m_invincibleTimer(0.0f)
    , m_onDeathCallback(nullptr)
    , m_onDamageCallback(nullptr)
    , m_onHealCallback(nullptr)
{
}

ComponentID HealthComponent::GetID() const
{
    return ComponentID::Health;
}

void HealthComponent::Setup(int maxHP, float initialInvincibilitySec)
{
    m_maxHP = maxHP;
    m_currentHP = maxHP;
    m_dead = false; // 再設定時に生き返るように
    m_invincibleTimer = initialInvincibilitySec;
}

void HealthComponent::Update(float deltaTime)
{
    if (m_invincibleTimer > 0.0f)
    {
        m_invincibleTimer -= deltaTime;
    }
}

void HealthComponent::TakeDamage(int amount)
{
    // 無敵または既に死亡している場合はダメージを受けない
    if (IsInvincible() || IsDead())
    {
        return;
    }

    m_currentHP -= amount;
    if (m_onDamageCallback)
    {
        m_onDamageCallback(amount);
    }

    if (m_currentHP <= 0)
    {
        m_currentHP = 0;
        m_dead = true;
        if (m_onDeathCallback)
        {
            m_onDeathCallback();
        }
    }
    else
    {
        // ダメージを受けたら、短い無敵時間を付与する
        SetInvincible(0.5f);
    }
}

void HealthComponent::Heal(int amount)
{
    if (IsDead()) return;

    m_currentHP += amount;
    m_currentHP = std::min(m_currentHP, m_maxHP); // m_maxHPを超えないように

    if (m_onHealCallback)
    {
        m_onHealCallback(amount);
    }
}

void HealthComponent::SetInvincible(float durationSec)
{
    // 現在の無敵時間より長い時間が指定された場合のみ更新する
    m_invincibleTimer = std::max(m_invincibleTimer, durationSec);
}

// --- ゲッター ---
int HealthComponent::GetHP() const { return m_currentHP; }
int HealthComponent::GetMaxHP() const { return m_maxHP; }
bool HealthComponent::IsAlive() const { return !m_dead; }
bool HealthComponent::IsDead() const { return m_dead; }
bool HealthComponent::IsInvincible() const { return m_invincibleTimer > 0.0f; }
float HealthComponent::GetInvincibleTimeRemaining() const { return m_invincibleTimer; }

// --- コールバックのセッター ---
void HealthComponent::SetOnDeathCallback(const std::function<void()>& callback) { m_onDeathCallback = callback; }
void HealthComponent::SetOnDamageCallback(const std::function<void(int)>& callback) { m_onDamageCallback = callback; }
void HealthComponent::SetOnHealCallback(const std::function<void(int)>& callback) { m_onHealCallback = callback; }