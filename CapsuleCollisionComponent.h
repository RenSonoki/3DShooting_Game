#pragma once
#include "ICollisionComponent.h"
#include <memory>
#include <utility> // for std::pair

class TransformComponent;

// カプセル形状のコライダーコンポーネント（リファクタリング後）
class CapsuleCollisionComponent : public ICollisionComponent
{
public:
    explicit CapsuleCollisionComponent(float radius, float height);
    virtual ~CapsuleCollisionComponent() = default;

    ComponentID GetID() const override;

    // --- ライフサイクルメソッド ---
    void Start() override;
    void Update(float deltaTime) override {}

    // --- ICollisionComponentインターフェースの実装 ---
    CollisionShapeType GetShapeType() const override { return CollisionShapeType::Capsule; }
    VECTOR GetCenter() const override;

    // --- このクラス固有の機能 ---
    float GetRadius() const;
    float GetHeight() const;

    // ワールド空間でのカプセルの中心線（線分）を取得する
    std::pair<VECTOR, VECTOR> GetWorldLineSegment() const;

    void SetBaseRadius(float radius);
    void SetBaseHeight(float height);

private:
    float m_baseRadius;
    float m_baseHeight;
    std::weak_ptr<TransformComponent> m_transform;
};