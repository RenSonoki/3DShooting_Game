#pragma once
#include "ComponentBase.h"
#include <memory>
#include "DxLib.h" // VECTORのため

// 前方宣言
class TransformComponent;
class CameraComponent;
struct Quaternion; // ★ Quaternionを前方宣言

class ThirdPersonCameraComponent : public ComponentBase
{
public:
    ThirdPersonCameraComponent();
    virtual ~ThirdPersonCameraComponent() = default;

    void Start() override;
    void Update(float deltaTime) override;
    ComponentID GetID() const override;

    // 追従するターゲット（プレイヤー）と、その設定
    void SetTarget(std::weak_ptr<TransformComponent> target);
    void SetDistance(float distance);
    void SetOffset(const VECTOR& offset);

private:
    std::weak_ptr<TransformComponent> m_transform;
    std::weak_ptr<CameraComponent> m_camera;
    std::weak_ptr<TransformComponent> m_target;

    float m_idealDistance;
    VECTOR m_targetOffset;
    float m_rotationSpeed;
    float m_lerpSpeed;

    float m_yaw;
    float m_pitch;
};