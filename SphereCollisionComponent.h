#pragma once
#include "ICollisionComponent.h" // 私たちがリファクタリングしたICollisionComponent
#include <memory>

class TransformComponent;

// 球体衝突コンポーネント
class SphereCollisionComponent : public ICollisionComponent
{
public:
    SphereCollisionComponent();
    virtual ~SphereCollisionComponent() = default;

    ComponentID GetID() const override;

    // --- ComponentBaseのライフサイクルメソッド ---
    void Start() override;
    // UpdateはColliderManagerが一括で処理するため、個々には不要
    void Update(float deltaTime) override {}

    // --- ICollisionComponentインターフェースの実装 ---
    CollisionShapeType GetShapeType() const override { return CollisionShapeType::Sphere; }
    VECTOR GetCenter() const override;

    // --- このクラス固有の機能 ---
    float GetRadius() const;
    void SetRadius(float radius);

private:
    float m_radius;
    // TransformComponentへのポインタはweak_ptrにすると、より安全です
    std::weak_ptr<TransformComponent> m_transform;
};