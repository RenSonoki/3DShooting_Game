#include "CameraComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <cassert>

CameraComponent::CameraComponent()
    : // モデルのスケールが大きいことを考慮し、描画範囲を調整
    m_fov(DX_PI_F / 4.0f),       // 視野角 (45度)
    m_nearClip(5.0f),            // 近い描画距離
    m_farClip(20000.0f)          // 遠い描画距離
{
}

ComponentID CameraComponent::GetID() const { return ComponentID::Camera; }

void CameraComponent::SetFov(float fovAngleRad) { m_fov = fovAngleRad; }
void CameraComponent::SetNearFarClip(float nearClip, float farClip)
{
    m_nearClip = nearClip;
    m_farClip = farClip;
}

void CameraComponent::Activate() const
{
    // 射影（レンズ）とビュー（位置と向き）の両方を設定
    ApplyProjectionSettings();
    ApplyViewSettings();
}

// private:
void CameraComponent::ApplyProjectionSettings() const
{
    // 💡 ★★★ 最も重要な修正点 ★★★
    // 複雑な行列計算は行わず、DxLibの基本的な自動設定機能を使います。
    // 現在の描画先（VirtualScreenManagerが設定した仮想スクリーン）の
    // サイズに合わせて、アスペクト比が自動で設定されます。
    SetupCamera_Perspective(m_fov);
    SetCameraNearFar(m_nearClip, m_farClip);
}

// private:
void CameraComponent::ApplyViewSettings() const
{
    // この部分は完璧なので変更ありません
    auto owner = GetOwner();
    if (!owner) return;
    auto transform = owner->GetComponent<TransformComponent>();
    if (!transform) return;

    VECTOR position = transform->GetPosition();
    VECTOR forward = transform->GetForward();
    VECTOR target = VAdd(position, forward);
    VECTOR up = transform->GetUp();

    // あなたの環境で動作する、基本的なカメラ設定関数
    SetCameraPositionAndTargetAndUpVec(position, target, up);
}