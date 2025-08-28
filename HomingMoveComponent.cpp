#include "HomingMoveComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <DxLib.h>
#include <cassert>

HomingMoveComponent::HomingMoveComponent()
    : m_selfTransform(nullptr)
    , m_targetTransform() // weak_ptrはデフォルトコンストラクタで空になる
    , m_speed(DEFAULT_SPEED)
{
}

ComponentID HomingMoveComponent::GetID() const
{
    return ComponentID::Homing;
}

void HomingMoveComponent::Start()
{
    // 自身のTransformComponentは頻繁に使うため、最初にキャッシュしておきます。
    m_selfTransform = GetOwner()->GetComponent<TransformComponent>();
    assert(m_selfTransform != nullptr && "HomingMoveComponent requires a TransformComponent on its owner.");
}

void HomingMoveComponent::SetTarget(std::shared_ptr<TransformComponent> target)
{
    // ターゲットを弱参照(weak_ptr)で保持することで、
    // 循環参照を防ぎ、ターゲットが破棄されたことを安全に検知できます。
    m_targetTransform = target;
}

void HomingMoveComponent::SetSpeed(float speed)
{
    m_speed = speed;
}

float HomingMoveComponent::GetSpeed() const
{
    return m_speed;
}

void HomingMoveComponent::Update(float deltaTime)
{
    // weak_ptrからshared_ptrを取得し、ターゲットが有効か確認します。
    // このスコープを抜けると、ロックしたポインタは自動的に解放されます。
    auto target = m_targetTransform.lock();

    // 自身、またはターゲットが存在しない場合は、その場で停止します。
    if (!m_selfTransform || !target)
    {
        if (m_selfTransform)
        {
            m_selfTransform->SetVelocity(VGet(0.0f, 0.0f, 0.0f));
        }
        return;
    }

    // 自身とターゲットの座標を取得します。
    const VECTOR currentPos = m_selfTransform->GetPosition();
    const VECTOR targetPos = target->GetPosition();

    // ターゲットへの方向ベクトルを計算します。
    const VECTOR dir = VSub(targetPos, currentPos);

    // 非常に近い距離にいる場合は、振動を防ぐために停止します。
    // VSize()の平方根計算を避けるため、距離の2乗で比較するのが効率的です。
    if (VSquareSize(dir) < 1e-6f) // 1e-6fは十分に小さい値の例
    {
        m_selfTransform->SetVelocity(VGet(0.0f, 0.0f, 0.0f));
        return;
    }

    // 方向ベクトルを正規化（長さを1に）します。
    const VECTOR normalizedDir = VNorm(dir);

    // 最終的な速度ベクトルを計算し、TransformComponentに設定します。
    const VECTOR velocity = VScale(normalizedDir, m_speed);
    m_selfTransform->SetVelocity(velocity);
}