#include "SphereCollisionComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <algorithm>
#include <cassert>

SphereCollisionComponent::SphereCollisionComponent()
    : m_radius(1.0f) // デフォルトの半径を1.0に設定
{
}

ComponentID SphereCollisionComponent::GetID() const
{
    return ComponentID::Sphere;
}

void SphereCollisionComponent::Start()
{
    // オーナーからTransformComponentを取得し、weak_ptrで保持
    m_transform = GetOwner()->GetComponent<TransformComponent>();
    assert(!m_transform.expired() && "SphereCollisionComponent requires a TransformComponent.");
}

VECTOR SphereCollisionComponent::GetCenter() const
{
    if (auto transform = m_transform.lock())
    {
        return transform->GetPosition();
    }
    // Transformが見つからない場合は、自身のオーナーの位置を返すなどのフォールバック
    if (auto owner = GetOwner())
    {
        // GetOwnerが返すのはEntityのshared_ptrなので、さらにTransformを取得
        if (auto ownerTransform = owner->GetComponent<TransformComponent>())
        {
            return ownerTransform->GetPosition();
        }
    }
    return VGet(0, 0, 0);
}

float SphereCollisionComponent::GetRadius() const
{
    if (auto transform = m_transform.lock())
    {
        VECTOR scale = transform->GetScale();
        float maxScale = (std::max)({ scale.x, scale.y, scale.z }); // C++11以降の書き方
        return m_radius * maxScale;
    }
    return m_radius;
}

void SphereCollisionComponent::SetRadius(float radius)
{
    m_radius = radius;
}
