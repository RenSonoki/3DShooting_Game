#pragma once//

#include <memory>
#include <string>
#include "Entity.h"
#include "TransformComponent.h"

// Entity を複製するためのインターフェース（Prototype パターンで生成ロジックを分離）
class IEntityPrototype
{
public:
    virtual ~IEntityPrototype() = default;

    // 完全複製（デフォルト位置）
    virtual std::shared_ptr<Entity> Clone() const = 0;

    // 位置付き複製（初期 Transform を指定）
    virtual std::shared_ptr<Entity> CloneWithTransform(const VECTOR& position) const = 0;

    // プロトタイプ名（UIでの表示や識別に使用）
    virtual std::wstring GetName() const = 0;

    // カテゴリ名（敵/弾/アイテムなどで分類）
    virtual std::wstring GetCategory() const = 0;
};