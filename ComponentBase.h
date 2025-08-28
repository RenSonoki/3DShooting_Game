#pragma once
#include "IComponent.h"

// 前方宣言
class Entity;

// コンポーネント実装のための共通基底クラス
// NOTE: IComponentの基本的なインターフェースを実装
class ComponentBase : public IComponent
{
protected:
    std::weak_ptr<Entity> m_owner;
    bool m_isActive = true;

public:
    virtual ~ComponentBase() = default;

    // --- 基本機能の実装 ---
    void SetOwner(std::weak_ptr<Entity> owner) override { m_owner = owner; }
    std::shared_ptr<Entity> GetOwner() const override { return m_owner.lock(); }

    void SetActive(bool active) override { m_isActive = active; }
    bool IsActive() const override { return m_isActive; }

    // NOTE: Start, Update, DrawはIComponentのデフォルト実装をそのまま継承します。
    //       GetID()は、このクラスを継承する具体的なコンポーネントが
    //       自身のIDを返す責任を持つため、ここでは実装しません。
};