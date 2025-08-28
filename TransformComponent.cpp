#include "TransformComponent.h"
#include <cmath> // Slerpなどで必要になる可能性があるため念のため

TransformComponent::TransformComponent()
    : m_position(VGet(0.0f, 0.0f, 0.0f))
    , m_velocity(VGet(0.0f, 0.0f, 0.0f))
    , m_rotation(Quaternion::Identity()) // 単位クォータニオンで初期化
    , m_scale(VGet(1.0f, 1.0f, 1.0f))
{
}

TransformComponent::TransformComponent(const VECTOR& pos)
    : m_position(pos)
    , m_velocity(VGet(0.0f, 0.0f, 0.0f))
    , m_rotation(Quaternion::Identity())
    , m_scale(VGet(1.0f, 1.0f, 1.0f))
{
}

void TransformComponent::Update(float deltaTime)
{
    if (VSquareSize(m_velocity) > 1e-8f)
    {
        m_position = VAdd(m_position, VScale(m_velocity, deltaTime));
    }
}

ComponentID TransformComponent::GetID() const
{
    return ComponentID::Transform;
}

MATRIX TransformComponent::GetWorldMatrix() const
{
    MATRIX rotMat = GetRotationMatrix();
    MATRIX scaleMat = MGetScale(m_scale);
    MATRIX transMat = MGetTranslate(m_position);

    // S -> R -> T の順で合成
    MATRIX worldMat = MMult(scaleMat, rotMat);
    return MMult(worldMat, transMat);
}

MATRIX TransformComponent::GetRotationMatrix() const
{
    return m_rotation.ToMatrix();
}

// --- Getters and Setters ---
const VECTOR& TransformComponent::GetPosition() const { return m_position; }
void TransformComponent::SetPosition(const VECTOR& pos) { m_position = pos; }

const VECTOR& TransformComponent::GetVelocity() const { return m_velocity; }
void TransformComponent::SetVelocity(const VECTOR& vel) { m_velocity = vel; }

const Quaternion& TransformComponent::GetRotation() const { return m_rotation; }
void TransformComponent::SetRotation(const Quaternion& rot)
{
    m_rotation = rot;
    m_rotation.Normalize();
}

void TransformComponent::SetRotation(const VECTOR& eulerAngles)
{
    m_rotation = Quaternion::FromEulerAngles(eulerAngles.x, eulerAngles.y, eulerAngles.z);
}

void TransformComponent::AddYaw(float angle)
{
    Quaternion yawRot = Quaternion::FromEulerAngles(0.0f, angle, 0.0f);
    m_rotation = m_rotation * yawRot; // クォータニオンの積で回転を合成
}

void TransformComponent::AddPitch(float angle)
{
    // PitchはローカルのX軸周りの回転として合成
    Quaternion pitchRot = Quaternion::FromEulerAngles(angle, 0.0f, 0.0f);
    m_rotation = m_rotation * pitchRot;
}

const VECTOR& TransformComponent::GetScale() const { return m_scale; }
void TransformComponent::SetScale(const VECTOR& scale) { m_scale = scale; }

// --- Direction Vectors ---
VECTOR TransformComponent::GetForward() const
{
    // Z軸プラスを「前」とする
    return VTransform(VGet(0.0f, 0.0f, 1.0f), GetRotationMatrix());
}

VECTOR TransformComponent::GetRight() const
{
    // X軸プラスを「右」とする
    return VTransform(VGet(1.0f, 0.0f, 0.0f), GetRotationMatrix());
}

VECTOR TransformComponent::GetUp() const
{
    // Y軸プラスを「上」とする【修正点】
    return VTransform(VGet(0.0f, 1.0f, 0.0f), GetRotationMatrix());
}