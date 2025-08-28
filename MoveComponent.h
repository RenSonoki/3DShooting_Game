#pragma once

#include "ComponentBase.h"
#include <DxLib.h>
#include <memory>

// 前方宣言
class TransformComponent;
class Entity;

/**
 * @class MoveComponent
 * @brief エンティティを一定方向に指定された速度で移動させるコンポーネント
 *
 * このコンポーネントは、オーナーエンティティのTransformComponentを操作して、
 * 継続的に移動させます。移動方向と速度は外部から設定可能です。
 */
class MoveComponent : public ComponentBase
{
public:
    /**
     * @brief コンストラクタ
     */
    MoveComponent();

    ComponentID GetID() const override;

    /**
     * @brief 初期化処理。オーナーから必要なコンポーネントを取得します。
     * このコンポーネントはTransformComponentに依存します。
     */
    void Start() override;

    /**
     * @brief 毎フレームの更新処理。速度を計算し、TransformComponentに適用します。
     * @param deltaTime 前のフレームからの経過時間（秒）
     */
    void Update(float deltaTime) override;

    /**
     * @brief 移動方向を設定します。ベクトルは自動的に正規化されます。
     * @param direction 新しい移動方向ベクトル
     */
    void SetDirection(const VECTOR& direction);

    /**
     * @brief 移動速度を設定します。
     * @param speed 新しい速度
     */
    void SetSpeed(float speed);

    /**
     * @brief 現在の移動方向を取得します。
     * @return 正規化された移動方向ベクトル
     */
    const VECTOR& GetDirection() const;

    /**
     * @brief 現在の移動速度を取得します。
     * @return 速度
     */
    float GetSpeed() const;

private:
    // --- 定数 ---
    static constexpr float DEFAULT_SPEED = 10.0f;
    static const VECTOR DEFAULT_DIRECTION;

    // --- メンバ変数 ---
    std::shared_ptr<TransformComponent> m_transform;
    VECTOR m_normalizedDirection; // 正規化済みの移動方向
    float m_speed;
};