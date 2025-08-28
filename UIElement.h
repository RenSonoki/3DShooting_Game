#pragma once
#include "IUIElement.h" // IUIElementのUpdateLogicも修正が必要です
#include <memory>
#include <string>

// 前方宣言
class UIRenderer;
class UIInteractor;
class UIAnimator;
struct Transform2D; // Transform2Dも前方宣言しておくと良いでしょう
struct VECTOR2;     // VECTOR2も前方宣言しておくと良いでしょう


class UIElement : public IUIElement
{
protected:
    Transform2D m_transform;
    bool m_isVisible = true;
    std::string m_name;
    int m_zOrder = 0;

    // 振る舞いを部品として所有
    std::unique_ptr<UIRenderer> m_renderer;
    std::unique_ptr<UIInteractor> m_interactor;
    std::unique_ptr<UIAnimator> m_animator;

public:
    UIElement();
    virtual ~UIElement();

    // --- IUIElement インターフェースの実装 ---
    void SetVisible(bool visible) override;
    bool IsVisible() const override;

    Transform2D& GetTransform() override;
    const Transform2D& GetTransform() const override;

    // ★ 修正点: deltaTime を引数として受け取るように変更
    // NOTE: 基底のIUIElement側も同様の修正が必要です
    void UpdateLogic(float deltaTime) override;
    void Draw() const override;

    // --- UIElement 固有の機能 ---
    virtual void UpdateInteraction();
    const std::string& GetName() const;
    void SetName(const std::string& name);

    void SetZOrder(int order);
    int GetZOrder() const;

    virtual VECTOR2 GetBoundingSize() const;

    bool HasRenderer() const;

    // 振る舞いを設定するメソッド
    void SetRenderer(std::unique_ptr<UIRenderer> renderer);
    void SetInteractor(std::unique_ptr<UIInteractor> interactor);
    void SetAnimator(std::unique_ptr<UIAnimator> animator);
};