// SimpleSpriteRendererに移管済み

//#pragma once
//
//#include "UIComponent.h"
//#include "SpriteRenderer.h"
//#include <string>
//
//// SpriteRendererを内包し、UIシステムに統合するためのコンポーネント
//class SpriteComponent : public UIComponent
//{
//public:
//    explicit SpriteComponent(const std::wstring& imagePath);
//    virtual ~SpriteComponent() = default;
//
//    // UIComponentの純粋仮想関数であるDrawをオーバーライド
//    void Draw(int targetScreen = -1) override;
//
//    // 内部のSpriteRendererにアクセスしたい場合（任意）
//    const SpriteRenderer& GetRenderer() const { return m_renderer; }
//    SpriteRenderer& GetRenderer() { return m_renderer; }
//
//private:
//    // SpriteRendererをメンバーとして保持（has-a関係）
//    SpriteRenderer m_renderer;
//};