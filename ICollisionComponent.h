#pragma once
#include "ComponentBase.h"
#include "CollisionShapeType.h"
#include <functional>
#include <memory>
#include <DxLib.h> // VECTORのため

// 前方宣言
class Entity;

// 【修正】基底クラスを ComponentBase に変更
class ICollisionComponent : public ComponentBase, public std::enable_shared_from_this<ICollisionComponent>
{
public:
    virtual ~ICollisionComponent();

    // 【改善案】ビジターパターンを廃止し、自身の形状タイプを返すシンプルなメソッドにする
    virtual CollisionShapeType GetShapeType() const = 0;

    // 当たり判定に必要な、形状の中心位置を返す
    virtual VECTOR GetCenter() const = 0;

    // 衝突イベントのコールバックを設定
    void SetOnCollision(std::function<void(std::shared_ptr<Entity>)> callback);
    // 衝突イベントを発火させる
    void TriggerCollision(std::shared_ptr<Entity> other) const;

    // ComponentBaseのSetOwnerをオーバーライドして、ColliderManagerへの登録を行う
    void SetOwner(std::weak_ptr<Entity> owner) override;

protected:
    // コールバック関数
    std::function<void(std::shared_ptr<Entity>)> m_onCollision;
};