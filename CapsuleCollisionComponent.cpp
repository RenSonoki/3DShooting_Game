#include "CapsuleCollisionComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <algorithm>
#include <cassert>

CapsuleCollisionComponent::CapsuleCollisionComponent(float radius, float height)
    : m_baseRadius(radius), m_baseHeight(height)
{
}

ComponentID CapsuleCollisionComponent::GetID() const
{
    return ComponentID::Capsule;
}

void CapsuleCollisionComponent::Start()
{
    m_transform = GetOwner()->GetComponent<TransformComponent>();
    assert(!m_transform.expired() && "CapsuleCollisionComponent requires a TransformComponent.");
}

VECTOR CapsuleCollisionComponent::GetCenter() const
{
    if (auto transform = m_transform.lock())
    {
        return transform->GetPosition();
    }
    return VGet(0, 0, 0);
}

float CapsuleCollisionComponent::GetRadius() const
{
    if (auto transform = m_transform.lock())
    {
        const auto& scale = transform->GetScale();
        return m_baseRadius * (std::max)({ scale.x, scale.y, scale.z });
    }
    return m_baseRadius;
}

float CapsuleCollisionComponent::GetHeight() const
{
    if (auto transform = m_transform.lock())
    {
        return m_baseHeight * transform->GetScale().y;
    }
    return m_baseHeight;
}

void CapsuleCollisionComponent::SetBaseRadius(float radius) { m_baseRadius = radius; }
void CapsuleCollisionComponent::SetBaseHeight(float height) { m_baseHeight = height; }

std::pair<VECTOR, VECTOR> CapsuleCollisionComponent::GetWorldLineSegment() const
{
    if (auto transform = m_transform.lock())
    {
        // 1. カプセルの中心線を、まずローカル空間で定義する (Y軸に沿う)
        const float halfHeight = m_baseHeight / 2.0f;
        const VECTOR localStart = VGet(0.0f, -halfHeight, 0.0f);
        const VECTOR localEnd = VGet(0.0f, halfHeight, 0.0f);

        // 2. オブジェクトのワールド行列を取得
        const MATRIX worldMat = transform->GetWorldMatrix();

        // 3. ローカルの始点と終点をワールド行列で変換し、ワールド座標の線分を得る
        const VECTOR worldStart = VTransform(localStart, worldMat);
        const VECTOR worldEnd = VTransform(localEnd, worldMat);

        return { worldStart, worldEnd };
    }

    // Transformがなければ、原点にある線分として返す
    return { VGet(0,0,0), VGet(0,0,0) };
}