#pragma once
#include <memory>

// 前方宣言
class PlayerControllerComponent;
class ModelAnimatorComponent;
class TransformComponent;

/**
 * @interface IPlayerState
 * @brief プレイヤーの全ての「状態」クラスが継承するインターフェース
 */
class IPlayerState
{
public:
    virtual ~IPlayerState() = default;

    // この状態になった瞬間に呼ばれる
    virtual void OnEnter(PlayerControllerComponent* owner) = 0;
    // この状態の間、毎フレーム呼ばれる
    virtual void Update(PlayerControllerComponent* owner, float deltaTime) = 0;
    // この状態から別の状態に移る瞬間に呼ばれる
    virtual void OnExit(PlayerControllerComponent* owner) = 0;
};


/**
 * @class PlayerIdleState
 * @brief 待機状態
 */
class PlayerIdleState : public IPlayerState
{
public:
    void OnEnter(PlayerControllerComponent* owner) override;
    void Update(PlayerControllerComponent* owner, float deltaTime) override;
    void OnExit(PlayerControllerComponent* owner) override;
};


/**
 * @class PlayerMoveState
 * @brief 移動状態
 */
class PlayerMoveState : public IPlayerState
{
public:
    void OnEnter(PlayerControllerComponent* owner) override;
    void Update(PlayerControllerComponent* owner, float deltaTime) override;
    void OnExit(PlayerControllerComponent* owner) override;
};