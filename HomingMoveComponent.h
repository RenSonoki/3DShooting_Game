#pragma once

#include "ComponentBase.h"
#include <memory>

// 前方宣言
class TransformComponent;

/**
 * @class HomingMoveComponent
 * @brief 特定の対象を追尾して移動するコンポーネント
 */
class HomingMoveComponent : public ComponentBase
{
public:
    /**
     * @brief コンストラクタ
     */
    HomingMoveComponent();

    ComponentID GetID() const override;

    /**
     * @brief 初期化処理。自身のTransformComponentを取得します。
     */
    void Start() override;

    /**
     * @brief 毎フレームの更新。ターゲットの方向を向き、速度を更新します。
     * @param deltaTime 前のフレームからの経過時間（秒）
     */
    void Update(float deltaTime) override;

    /**
     * @brief 追尾対象のTransformComponentを設定します。
     * @param target 追尾したいエンティティのTransformComponent
     */
    void SetTarget(std::shared_ptr<TransformComponent> target);

    /**
     * @brief 追尾速度を設定します。
     * @param speed 追尾速度
     */
    void SetSpeed(float speed);

    /**
     * @brief 現在の追尾速度を取得します。
     * @return 追尾速度
     */
    float GetSpeed() const;

private:
    // --- 定数 ---
    static constexpr float DEFAULT_SPEED = 50.0f;

    // --- メンバ変数 ---
    std::shared_ptr<TransformComponent> m_selfTransform;
    std::weak_ptr<TransformComponent> m_targetTransform;
    float m_speed;
};