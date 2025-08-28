#pragma once
#include "ComponentBase.h"
#include <memory>

// 前方宣言
class TransformComponent;
class CameraComponent;
class ModelAnimatorComponent;
class IPlayerState;
class BulletShooterComponent;

/**
 * @class PlayerControllerComponent
 * @brief プレイヤーの状態を管理し、状態に応じた処理の実行を委任するコンポーネント
 */
class PlayerControllerComponent : public ComponentBase
{
public:
    PlayerControllerComponent();
    virtual ~PlayerControllerComponent();

    // --- ライフサイクルメソッド ---
    void Start() override;
    void Update(float deltaTime) override;
    ComponentID GetID() const override;

    /**
     * @brief プレイヤーの状態を新しい状態に遷移させます
     * @param newState 新しい状態オブジェクト
     */
    void ChangeState(std::unique_ptr<IPlayerState> newState);

    /**
     * @brief 外部（例：GameScene）からカメラを設定します
     * @param camera メインカメラのCameraComponent
     */
    void SetCamera(std::weak_ptr<CameraComponent> camera);

    // --- 状態クラス（State）から各種情報を取得するためのゲッター ---
    TransformComponent* GetTransform() const;
    CameraComponent* GetCamera() const;
    ModelAnimatorComponent* GetAnimator() const;
    BulletShooterComponent* GetShooter() const;
    float GetMoveSpeed() const;
    float GetRotationSpeed() const;

private:
    // 連携するコンポーネントへのポインタ
    std::weak_ptr<TransformComponent> m_transform;
    std::weak_ptr<CameraComponent> m_camera;
    std::weak_ptr<ModelAnimatorComponent> m_animator;
    std::weak_ptr<BulletShooterComponent> m_shooter;

    // 現在の状態を保持するポインタ
    std::unique_ptr<IPlayerState> m_currentState;

    // パラメータ
    float m_moveSpeed;
    float m_rotationSpeed;
};