#include "ThirdPersonCameraComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Quaternion.h"
#include "DebugRenderer.h"
#include <cassert>
#include <algorithm> // std::clamp
#include <cmath>     // std::exp

// ★ 引数をconst参照に変更
static MATRIX CreateMatrixFromForwardVector(const VECTOR& forward, const VECTOR& up)
{
    VECTOR z_axis = VNorm(forward);
    VECTOR x_axis = VNorm(VCross(up, z_axis));
    VECTOR y_axis = VCross(z_axis, x_axis);

    MATRIX m = MGetIdent();
    m.m[0][0] = x_axis.x; m.m[0][1] = x_axis.y; m.m[0][2] = x_axis.z;
    m.m[1][0] = y_axis.x; m.m[1][1] = y_axis.y; m.m[1][2] = y_axis.z;
    m.m[2][0] = z_axis.x; m.m[2][1] = z_axis.y; m.m[2][2] = z_axis.z;
    return m;
}

ThirdPersonCameraComponent::ThirdPersonCameraComponent()
    : m_idealDistance(40.0f) // 距離を少し離す
    , m_targetOffset(VGet(0, 5.0f, 0))
    , m_rotationSpeed(2.0f)
    , m_lerpSpeed(15.0f)
    , m_yaw(DX_PI_F * 0.25f) // 水平回転(Yaw): プレイヤーの左45度の位置にカメラを配置
    , m_pitch(-0.2f)         // 垂直回転(Pitch): より真上から見下ろすように角度を深くする
{
}

ComponentID ThirdPersonCameraComponent::GetID() const
{
    // ComponentIDにThirdPersonCameraなどを追加してください
    return ComponentID::ThirdPersonCamera;
}


void ThirdPersonCameraComponent::Start()
{
    m_transform = GetOwner()->GetComponent<TransformComponent>();
    m_camera = GetOwner()->GetComponent<CameraComponent>();
    assert(!m_transform.expired() && "ThirdPersonCameraComponent requires a TransformComponent.");
    assert(!m_camera.expired() && "ThirdPersonCameraComponent requires a CameraComponent.");
}

void ThirdPersonCameraComponent::SetTarget(std::weak_ptr<TransformComponent> target) { m_target = target; }
void ThirdPersonCameraComponent::SetDistance(float distance) { m_idealDistance = distance; }
void ThirdPersonCameraComponent::SetOffset(const VECTOR& offset) { m_targetOffset = offset; }


void ThirdPersonCameraComponent::Update(float deltaTime)
{
    auto transform = m_transform.lock();
    auto target = m_target.lock();
    if (!transform || !target) return;

    // --- 1. 入力によるカメラの回転 ---
    // (InputManagerなどからマウスの移動差分を取得する)
    // float mouseDeltaX = InputManager::GetInstance().GetMouseDeltaX();
    // m_yaw += mouseDeltaX * m_rotationSpeed * deltaTime; // ★ deltaTimeを考慮
    m_pitch = std::clamp(m_pitch, -0.5f, 1.5f);

    // --- 2. カメラの理想的な位置を計算 ---
    Quaternion rotation = Quaternion::FromEulerAngles(m_pitch, m_yaw, 0.0f);
    VECTOR direction = VTransform(VGet(0, 0, -1), rotation.ToMatrix());
    VECTOR idealPosition = VAdd(target->GetPosition(), VScale(direction, m_idealDistance));

    // --- 3. 障害物チェック ---
    VECTOR finalPosition = idealPosition;
    // Physics::Raycast(...) のような処理がここに入る

    // --- 4. カメラのTransformを滑らかに更新 ---
    // ★ 修正点: フレームレートに依存しない安定したLerp（線形補間）
    VECTOR currentPos = transform->GetPosition();
    float factor = 1.0f - std::exp(-m_lerpSpeed * deltaTime);
    transform->SetPosition(VAdd(currentPos, VScale(VSub(finalPosition, currentPos), factor)));

    // 向きの更新 (常にターゲットの注視点を向く)
    VECTOR lookAtTarget = VAdd(target->GetPosition(), m_targetOffset);
    VECTOR lookDir = VSub(lookAtTarget, transform->GetPosition());

#if _DEBUG
    // ★ デバッグ描画をリクエスト
    // カメラの位置に青い球を描画
    DebugRenderer::GetInstance().AddSphere(transform->GetPosition(), 1.0f, GetColor(0, 0, 255));
    // カメラから注視点へ黄色い線を描画
    DebugRenderer::GetInstance().AddLine(transform->GetPosition(), lookAtTarget, GetColor(255, 255, 0));
#endif

    if (VSquareSize(lookDir) > 1e-6f)
    {
        MATRIX lookAtMatrix = CreateMatrixFromForwardVector(lookDir, VGet(0, 1, 0));
        Quaternion targetRot = Quaternion::FromMatrix(lookAtMatrix);
        Quaternion currentRot = transform->GetRotation();

        // ★ 修正点: Slerpの補間率もフレームレートから独立させる
        float rotFactor = 1.0f - std::exp(-m_lerpSpeed * 1.5f * deltaTime);
        Quaternion newRot = Quaternion::Slerp(currentRot, targetRot, rotFactor);

        transform->SetRotation(newRot);
    }
}