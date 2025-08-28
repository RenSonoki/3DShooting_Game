// UIElementに移管済み


//#pragma once

//
//#include "ComponentBase.h"
//#include "IUIRenderable.h"
//#include "IUITransformable.h"
//#include "IUIVisible.h"
//#include "Transform2D.h"
//#include <string>
//
//class Entity;
//
//
//class UIComponent : public ComponentBase, public IUIRenderable, public IUITransformable, public IUIVisible
//{
//public:
//    UIComponent();
//    virtual ~UIComponent() = default;
//
//    // --- ComponentBaseが共通機能を提供するため、以下の実装は不要になる ---
//    // void SetOwner(Entity* entity) override;
//    // Entity* GetOwner() const override;
//    // void SetActive(bool active) override;
//    // bool IsActive() const override;
//
//    // --- UI固有の機能 ---
//
//    // 表示制御
//    void SetVisible(bool visible) override;
//    bool IsVisible() const override;
//
//    // Transform
//    Transform2D& GetTransform() override;
//    const Transform2D& GetTransform() const override;
//
//    // Z順制御
//    void SetZOrder(int order);
//    int GetZOrder() const;
//
//    // 名前
//    void SetName(const std::string& name);
//    const std::string& GetName() const;
//
//    // 描画（これを継承するクラスが具体的な描画処理を実装する）
//    void Draw(int targetScreen = -1) override = 0;
//
//protected:
//    // m_owner, m_active は ComponentBase が持っているので削除
//    bool m_visible = true;
//    Transform2D m_transform;
//    int m_zOrder = 0;
//    std::string m_name;
//};