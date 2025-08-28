#pragma once

#include "ComponentBase.h"
#include <functional>

/**
 * @class LifetimeComponent
 * @brief エンティティの寿命を管理し、寿命が尽きた時に特定のアクションを実行するコンポーネント
 */
class LifetimeComponent : public ComponentBase
{
public:
    /**
     * @brief コンストラクタ
     */
    LifetimeComponent();

    ComponentID GetID() const override;

    /**
     * @brief 毎フレームの更新。寿命を減算し、0以下になったら処理を実行します。
     * @param deltaTime 前のフレームからの経過時間（秒）
     */
    void Update(float deltaTime) override;

    /**
     * @brief 寿命を秒数で設定します。
     * @param lifetimeInSeconds 寿命（秒）。負の値を設定すると永続します。
     */
    void SetLifetime(float lifetimeInSeconds);

    /**
     * @brief 寿命が尽きたかどうかを返します。
     * @return trueの場合、寿命が尽きている
     */
    bool IsExpired() const;

    /**
     * @brief 寿命が尽きた時に呼び出されるコールバック関数を設定します。
     * @param callback 実行したい処理を記述した関数
     */
    void SetOnExpired(const std::function<void()>& callback);

private:
    float m_lifetime;
    bool m_expired;

    // 寿命が尽きた時に実行されるカスタム処理
    std::function<void()> m_onExpiredCallback;
};