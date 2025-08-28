#pragma once
#include "ComponentBase.h"
#include <DxLib.h>

class CameraComponent : public ComponentBase
{
public:
    CameraComponent();
    virtual ~CameraComponent() = default;

    void Update(float deltaTime) override {}
    ComponentID GetID() const override;

    // このカメラをDxLibのアクティブなカメラとして設定する
    void Activate() const;

    void SetFov(float fovAngleRad);
    void SetNearFarClip(float nearClip, float farClip);

private:
    void ApplyProjectionSettings() const;
    void ApplyViewSettings() const;

    float m_fov;
    float m_nearClip;
    float m_farClip;
};