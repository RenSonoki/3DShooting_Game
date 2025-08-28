#include "UIElement.h"
#include "UIRenderer.h"
#include "UIInteractor.h"
#include "UIAnimator.h"      // ★ 修正点: UIAnimatorの定義を読み込む
#include "Transform2D.h"     // Transform2DやVECTOR2の定義に必要

// ★ 修正点: コンストラクタの「定義」をここに移動
UIElement::UIElement() = default;

// ★ 修正点: デストラクタの「定義」をここに移動
// この時点では、UIAnimatorなどの型は「完全な型」になっているため、
// unique_ptrは安全にオブジェクトを破棄できる。
UIElement::~UIElement() = default;

void UIElement::SetVisible(bool visible) { m_isVisible = visible; }
bool UIElement::IsVisible() const { return m_isVisible; }

Transform2D& UIElement::GetTransform() { return m_transform; }
const Transform2D& UIElement::GetTransform() const { return m_transform; }

// ★ 修正点: deltaTime を引数として受け取るように変更
void UIElement::UpdateLogic(float deltaTime)
{
    // アニメーターが設定されていれば、アニメーションロジックを更新
    if (m_animator)
    {
        // 引数で受け取ったdeltaTimeを渡す
        m_animator->Update(this, m_transform, deltaTime);
    }
}

void UIElement::UpdateInteraction()
{
    if (m_isVisible && m_interactor)
    {
        m_interactor->UpdateInteraction(this);
    }
}

void UIElement::Draw() const
{
    if (m_isVisible && m_renderer)
    {
        m_renderer->Draw(this);
    }
}

const std::string& UIElement::GetName() const { return m_name; }
void UIElement::SetName(const std::string& name) { m_name = name; }

void UIElement::SetZOrder(int order) { m_zOrder = order; }
int UIElement::GetZOrder() const { return m_zOrder; }

VECTOR2 UIElement::GetBoundingSize() const
{
    // デフォルト実装
    return { 0.0f, 0.0f };
}

bool UIElement::HasRenderer() const
{
    // m_renderer (unique_ptr) がnullptrでなければ、描画機能を持っていると判断します
    return m_renderer != nullptr;
}

void UIElement::SetRenderer(std::unique_ptr<UIRenderer> renderer) { m_renderer = std::move(renderer); }
void UIElement::SetInteractor(std::unique_ptr<UIInteractor> interactor) { m_interactor = std::move(interactor); }
void UIElement::SetAnimator(std::unique_ptr<UIAnimator> animator) { m_animator = std::move(animator); }