#include "PlayerControllerComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ModelAnimatorComponent.h"
#include "BulletShooterComponent.h"
#include "DebugRenderer.h" // デバッグ描画用のヘッダーファイルをインクルード
#include "PlayerState.h" // 状態クラスの実装をインクルード
#include <cassert>

PlayerControllerComponent::PlayerControllerComponent()
    : m_moveSpeed(5.0f)
    , m_rotationSpeed(15.0f)
    , m_currentState(nullptr)
{
}

// ★ 修正点: デストラクタの「定義」をここに移動する
// この場所では IPlayerState が完全な型なので、m_currentState は安全に破棄できる
PlayerControllerComponent::~PlayerControllerComponent() = default;

void PlayerControllerComponent::Start()
{
    // 自身がアタッチされているエンティティから、連携するコンポーネントを取得してキャッシュ
    m_transform = GetOwner()->GetComponent<TransformComponent>();
    m_animator = GetOwner()->GetComponent<ModelAnimatorComponent>();
    m_shooter = GetOwner()->GetComponent<BulletShooterComponent>();

    // 連携するコンポーネントが設定されているか、デバッグ時にチェック
    assert(!m_transform.expired() && "PlayerControllerComponent requires a TransformComponent.");
    assert(!m_animator.expired() && "PlayerControllerComponent requires a ModelAnimatorComponent.");
    assert(!m_shooter.expired() && "PlayerControllerComponent requires a BulletShooterComponent.");

    // 最初の状態を「待機状態」に設定して開始
    ChangeState(std::make_unique<PlayerIdleState>());
}

void PlayerControllerComponent::Update(float deltaTime)
{
    // Updateの責務は、現在の状態オブジェクトのUpdateを呼び出すことだけ
    if (m_currentState)
    {
        m_currentState->Update(this, deltaTime);
    }

#if _DEBUG
    // ★ プレイヤーの位置に緑の球を描画
    if (auto tr = GetTransform()) {
        DebugRenderer::GetInstance().AddSphere(tr->GetPosition(), 1.5f, GetColor(0, 255, 0));
    }
#endif
}

ComponentID PlayerControllerComponent::GetID() const
{
    // ComponentIDにPlayerControllerなどを追加してください
    return ComponentID::PlayerController;
}

void PlayerControllerComponent::ChangeState(std::unique_ptr<IPlayerState> newState)
{
    // 現在の状態があれば、その終了処理を呼び出す
    if (m_currentState)
    {
        m_currentState->OnExit(this);
    }

    // 新しい状態に切り替える
    m_currentState = std::move(newState);

    // 新しい状態の開始処理を呼び出す
    if (m_currentState)
    {
        m_currentState->OnEnter(this);
    }
}

void PlayerControllerComponent::SetCamera(std::weak_ptr<CameraComponent> camera)
{
    m_camera = camera;
}

// --- ゲッターの実装 ---
// Stateクラスがオーナーの情報を安全に取得するために使います
TransformComponent* PlayerControllerComponent::GetTransform() const { return m_transform.lock().get(); }
CameraComponent* PlayerControllerComponent::GetCamera() const { return m_camera.lock().get(); }
ModelAnimatorComponent* PlayerControllerComponent::GetAnimator() const { return m_animator.lock().get(); }
BulletShooterComponent* PlayerControllerComponent::GetShooter() const { return m_shooter.lock().get(); }
float PlayerControllerComponent::GetMoveSpeed() const { return m_moveSpeed; }
float PlayerControllerComponent::GetRotationSpeed() const { return m_rotationSpeed; }