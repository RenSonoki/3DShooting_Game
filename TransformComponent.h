#pragma once
#include "ComponentBase.h"
#include "Quaternion.h" // 私たちが作成したQuaternionクラス
#include <DxLib.h>

class TransformComponent : public ComponentBase
{
public:
    TransformComponent();
    explicit TransformComponent(const VECTOR& pos);
    virtual ~TransformComponent() = default;

    void Update(float deltaTime) override;
    ComponentID GetID() const override;

    MATRIX GetWorldMatrix() const;

    const VECTOR& GetPosition() const;
    void SetPosition(const VECTOR& pos);

    const VECTOR& GetVelocity() const;
    void SetVelocity(const VECTOR& vel);

    // 回転をクォータニオンで直接設定・取得
    const Quaternion& GetRotation() const;
    void SetRotation(const Quaternion& rot);
    // オイラー角(ラジアン)から回転を設定（互換性のため）
    void SetRotation(const VECTOR& eulerAngles);

    // Y軸周りの回転（Yaw）を現在の回転に追加する
    void AddYaw(float angle);
    // X軸周りの回転（Pitch）を現在の回転に追加する
    void AddPitch(float angle);

    const VECTOR& GetScale() const;
    void SetScale(const VECTOR& scale);

    VECTOR GetForward() const;
    VECTOR GetRight() const;
    VECTOR GetUp() const;

private:
    MATRIX GetRotationMatrix() const;

    VECTOR m_position;
    VECTOR m_velocity;
    Quaternion m_rotation; // 内部表現をQuaternionに変更
    VECTOR m_scale;
};