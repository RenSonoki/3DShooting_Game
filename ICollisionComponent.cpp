#include "ICollisionComponent.h"
#include "ColliderManager.h"
#include "Entity.h" // Entityの完全な定義が必要

ICollisionComponent::~ICollisionComponent()
{
    // コンポーネントが破棄される際にColliderManagerから登録を解除する
    if (auto sharedThis = std::enable_shared_from_this<ICollisionComponent>::shared_from_this())
    {
        ColliderManager::GetInstance().Unregister(sharedThis);
    }
}

void ICollisionComponent::SetOnCollision(std::function<void(std::shared_ptr<Entity>)> callback)
{
    m_onCollision = std::move(callback);
}

void ICollisionComponent::TriggerCollision(std::shared_ptr<Entity> other) const
{
    if (m_onCollision)
    {
        m_onCollision(other);
    }
}

void ICollisionComponent::SetOwner(std::weak_ptr<Entity> owner)
{
    // まず、親クラスのSetOwnerを呼び出して、m_ownerを設定
    ComponentBase::SetOwner(owner);

    // 自身のshared_ptrを取得して、ColliderManagerに登録する
    // EntityのAddComponentがshared_ptrでComponentを生成していることが前提
    if (auto sharedThis = std::enable_shared_from_this<ICollisionComponent>::shared_from_this())
    {
        ColliderManager::GetInstance().Register(sharedThis);
    }
}