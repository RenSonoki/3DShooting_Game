#include "LifetimeComponent.h"
#include "Entity.h" // GetOwner()やSetActive()のために必要

LifetimeComponent::LifetimeComponent()
    : m_lifetime(-1.0f) // デフォルトは永続（負の値）
    , m_expired(false)
    , m_onExpiredCallback(nullptr)
{
}

ComponentID LifetimeComponent::GetID() const
{
    return ComponentID::Life;
}

void LifetimeComponent::SetLifetime(float lifetimeInSeconds)
{
    m_lifetime = lifetimeInSeconds;
    // 新しく寿命が設定されたら、失効フラグをリセット
    m_expired = (m_lifetime == 0.0f);
}

bool LifetimeComponent::IsExpired() const
{
    return m_expired;
}


void LifetimeComponent::Update(float deltaTime)
{
    // 既に寿命が尽きている、または永続（負の値）の場合は何もしない
    if (m_expired || m_lifetime < 0.0f)
    {
        return;
    }

    m_lifetime -= deltaTime;

    // 寿命が尽きた瞬間の処理
    if (m_lifetime <= 0.0f)
    {
        m_expired = true;

        // カスタムのコールバックが設定されていれば、それを呼び出す
        if (m_onExpiredCallback)
        {
            m_onExpiredCallback();
        }
        else
        {
            // コールバックが未設定の場合のデフォルト動作：オーナーを非アクティブにする
            // GetOwner()はshared_ptrを返すので、nullptrチェックが安全
            if (auto owner = GetOwner())
            {
                owner->SetActive(false);
            }
        }
    }
}

void LifetimeComponent::SetOnExpired(const std::function<void()>& callback)
{
    m_onExpiredCallback = callback;
}