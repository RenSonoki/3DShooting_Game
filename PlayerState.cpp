#include "PlayerState.h"
#include "PlayerControllerComponent.h"
#include "BulletShooterComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ModelAnimatorComponent.h"
#include "Quaternion.h"
#include "Entity.h"
#include "DxLib.h"
#include <cmath>

// ---- 待機状態 (Idle) ----
void PlayerIdleState::OnEnter(PlayerControllerComponent* owner)
{
    // 待機状態に入ったら、idleアニメーションを再生
    if (auto anim = owner->GetAnimator())
    {
        anim->PlayAnimationByName(L"idle", true);
    }
    // 速度をゼロにする
    if (auto tr = owner->GetTransform())
    {
        tr->SetVelocity({ 0,0,0 });
    }
}

void PlayerIdleState::Update(PlayerControllerComponent* owner, float deltaTime)
{
    if (GetMouseInput() & MOUSE_INPUT_LEFT) // マウスの左クリックをチェック
    {
        if (auto shooter = owner->GetShooter())
        {
            shooter->RequestShoot();
        }
    }

    // 移動入力があったかチェック
    if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_D))
    {
        // 入力があれば「移動状態」に遷移
        owner->ChangeState(std::make_unique<PlayerMoveState>());
    }
}

void PlayerIdleState::OnExit(PlayerControllerComponent* owner)
{
    // 特に何もする必要なし
}


// ---- 移動状態 (Move) ----
void PlayerMoveState::OnEnter(PlayerControllerComponent* owner)
{
    // 移動状態に入ったら、walkアニメーションを再生
    if (auto anim = owner->GetAnimator())
    {
        anim->PlayAnimationByName(L"walk", true);
    }
}

void PlayerMoveState::Update(PlayerControllerComponent* owner, float deltaTime)
{
    auto transform = owner->GetTransform();
    auto camera = owner->GetCamera();
    if (!transform || !camera) return;

    auto cameraTransform = camera->GetOwner()->GetComponent<TransformComponent>();
    if (!cameraTransform) return;

    // --- 向きの制御 (元のロジックと同じ) ---
    VECTOR aimDirection = cameraTransform->GetForward();
    aimDirection.y = 0.0f;
    if (VSquareSize(aimDirection) > 1e-6f)
    {
        aimDirection = VNorm(aimDirection);
        float targetAngle = atan2f(aimDirection.x, aimDirection.z);
        Quaternion targetRot = Quaternion::FromEulerAngles(0.0f, targetAngle, 0.0f);
        Quaternion currentRot = transform->GetRotation();
        Quaternion newRot = Quaternion::Slerp(currentRot, targetRot, deltaTime * owner->GetRotationSpeed());
        transform->SetRotation(newRot);
    }

    // --- 移動入力 (元のロジックと同じ) ---
    VECTOR inputVec = { 0,0,0 };
    if (CheckHitKey(KEY_INPUT_W)) { inputVec.z += 1.0f; }
    if (CheckHitKey(KEY_INPUT_S)) { inputVec.z -= 1.0f; }
    if (CheckHitKey(KEY_INPUT_A)) { inputVec.x -= 1.0f; }
    if (CheckHitKey(KEY_INPUT_D)) { inputVec.x += 1.0f; }

    // ★ 発射ロジックを追加
    if (GetMouseInput() & MOUSE_INPUT_LEFT) // マウスの左クリックをチェック
    {
        if (auto shooter = owner->GetShooter())
        {
            shooter->RequestShoot();
        }
    }

    // ★ 状態遷移のチェック
    if (VSquareSize(inputVec) < 1e-6f)
    {
        // 入力がなくなったら「待機状態」に遷移
        owner->ChangeState(std::make_unique<PlayerIdleState>());
        return; // 遷移したので、このフレームの以降の処理は不要
    }

    // --- 速度の設定 (元のロジックと同じ) ---
    VECTOR cameraForward = VNorm(VECTOR{ cameraTransform->GetForward().x, 0.0f, cameraTransform->GetForward().z });
    VECTOR cameraRight = VNorm(VECTOR{ cameraTransform->GetRight().x, 0.0f, cameraTransform->GetRight().z });
    VECTOR moveDir = VNorm(VAdd(VScale(cameraRight, inputVec.x), VScale(cameraForward, inputVec.z)));
    VECTOR velocity = VScale(moveDir, owner->GetMoveSpeed());
    transform->SetVelocity(velocity);
}

void PlayerMoveState::OnExit(PlayerControllerComponent* owner)
{
    // 状態を抜ける時に、念のため速度をゼロにしておく
    if (auto tr = owner->GetTransform())
    {
        tr->SetVelocity({ 0,0,0 });
    }
}