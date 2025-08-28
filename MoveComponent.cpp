#include "MoveComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <cassert>

// 静的メンバ変数の実体を定義
const VECTOR MoveComponent::DEFAULT_DIRECTION = VGet(0.0f, 0.0f, 1.0f);

MoveComponent::MoveComponent()
// メンバ変数を初期化リストで初期化します。
// 基底クラス(ComponentBase)のデフォルトコンストラクタは暗黙的に呼び出されます。
    : m_transform(nullptr)
    , m_normalizedDirection(DEFAULT_DIRECTION)
    , m_speed(DEFAULT_SPEED)
{
    // コンストラクタ本体は空でOKです。
    // オーナーの設定は、このコンポーネントを生成する側の責任です。
}

ComponentID MoveComponent::GetID() const
{
    // ComponentID列挙型にMoveなどを追加してください
    return ComponentID::Move;
}

void MoveComponent::Start()
{
    // このコンポーネントが機能するためにはTransformComponentが必須です。
    // GetOwner()は、事前にSetOwner()が呼ばれていることを前提とします。
    m_transform = GetOwner()->GetComponent<TransformComponent>();

    // 依存コンポーネントが見つからない場合は、開発中に即時発見できるようassertで停止させます。
    assert(m_transform != nullptr && "MoveComponent requires a TransformComponent on the same Entity.");
}

void MoveComponent::Update(float deltaTime)
{
    // TransformComponentがない、または速度が0以下の場合は何もしません。
    if (!m_transform || m_speed <= 0.0f)
    {
        // 停止している場合は、念のため速度を明示的に0にします。
        if (m_transform)
        {
            m_transform->SetVelocity(VGet(0.0f, 0.0f, 0.0f));
        }
        return;
    }

    // 方向と速度から速度ベクトルを算出し、TransformComponentに渡します。
    // 物理的な移動の責務はTransformComponentが持ちます。
    const VECTOR velocity = VScale(m_normalizedDirection, m_speed);
    m_transform->SetVelocity(velocity);
}

void MoveComponent::SetDirection(const VECTOR& direction)
{
    // 方向は常に正規化（長さを1に）して保持します。
    m_normalizedDirection = VNorm(direction);
}

void MoveComponent::SetSpeed(float speed)
{
    m_speed = speed;
}

const VECTOR& MoveComponent::GetDirection() const
{
    return m_normalizedDirection;
}

float MoveComponent::GetSpeed() const
{
    return m_speed;
}