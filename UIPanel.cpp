#include "UIPanel.h"
#include "UIElement.h" // UIElement の定義をインクルード
#include <algorithm> // std::sort のためにインクルード

UIPanel::UIPanel() = default;

// ★ 修正点: デストラクタの「定義」をここに移動する
// この場所では UIElement の完全な定義が見えているため、
// m_children が持つ unique_ptr は安全に中身を破棄できる。
UIPanel::~UIPanel() = default;

void UIPanel::AddChild(std::unique_ptr<UIElement> child)
{
    if (child)
    {
        m_children.push_back(std::move(child));
        m_isZOrderDirty = true; // ★ 改善案: 子要素が追加されたので再ソートが必要
    }
}

const std::vector<std::unique_ptr<UIElement>>& UIPanel::GetChildren() const
{
    return m_children;
}

// ★ 修正点: deltaTime引数を追加
void UIPanel::UpdateLogic(float deltaTime)
{
    if (!IsVisible()) return;

    // まず自分自身のロジックを更新
    UIElement::UpdateLogic(deltaTime);

    // 次に子要素のロジックを更新
    for (const auto& child : m_children)
    {
        if (child)
        {
            child->UpdateLogic(deltaTime);
        }
    }
}

// ★ 追加: Interaction更新の責務をこちらに分離
void UIPanel::UpdateInteraction()
{
    if (!IsVisible()) return;

    // まず自分自身のインタラクションを更新
    UIElement::UpdateInteraction();

    // 次に子要素のインタラクションを更新
    for (const auto& child : m_children)
    {
        if (child)
        {
            child->UpdateInteraction();
        }
    }
}


void UIPanel::Draw() const
{
    if (!IsVisible()) return;

    // まず自分自身を描画（パネルに背景色などがある場合）
    UIElement::Draw();

    // ★ 改善案: Zオーダーでソートして描画する
    // const_castを使っていますが、これはメンバー変数のフラグを更新するためだけで、
    // オブジェクトの外部から見た状態は変更しないため、論理的にはconst性を保っています。
    if (m_isZOrderDirty)
    {
        auto& children_ref = const_cast<std::vector<std::unique_ptr<UIElement>>&>(m_children);
        std::sort(children_ref.begin(), children_ref.end(),
            [](const std::unique_ptr<UIElement>& a, const std::unique_ptr<UIElement>& b) {
                return a->GetZOrder() < b->GetZOrder();
            });
        const_cast<UIPanel*>(this)->m_isZOrderDirty = false;
    }

    // Zオーダー順に子要素を描画
    for (const auto& child : m_children)
    {
        if (child)
        {
            child->Draw();
        }
    }
}

void UIPanel::SetVisible(bool visible)
{
    // 自身の可視性を設定
    UIElement::SetVisible(visible);

    // すべての子要素の可視性も再帰的に設定
    for (const auto& child : m_children)
    {
        if (child)
        {
            child->SetVisible(visible);
        }
    }
}