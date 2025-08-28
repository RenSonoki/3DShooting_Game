#pragma once
#include "ComponentID.h"
#include <memory>

// 前方宣言
class Entity;

// 全てのコンポーネントが従うべきインターフェース
class IComponent
{
public:
    virtual ~IComponent() = default;

    // --- 基本機能 ---
    // NOTE: EntityクラスのAddComponentメソッドから呼び出されることを想定
    //       所有者を std::weak_ptr で安全に設定・取得
    virtual void SetOwner(std::weak_ptr<Entity> owner) = 0;
    virtual std::shared_ptr<Entity> GetOwner() const = 0;

    virtual void SetActive(bool active) = 0;
    virtual bool IsActive() const = 0;

    // --- ライフサイクルメソッド ---
    // NOTE: デフォルトの空実装を持たせることで、派生クラスは必要なものだけオーバーライド。

    // Entityに追加され、準備が整った時に一度だけ呼ばれる
    virtual void Start() {}

    // 毎フレーム呼ばれる更新処理
    virtual void Update(float deltaTime) {}

    // 毎フレーム呼ばれる描画処理
    // NOTE: 描画は状態を変更しないはずなので const を付けます
    virtual void Draw() const {}

    // --- 型情報 ---
    // NOTE: 派生クラスは必ず自身のIDを返すように強制
    virtual ComponentID GetID() const = 0;
};