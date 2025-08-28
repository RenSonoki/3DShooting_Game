#pragma once
#include "UIElement.h"
#include <vector>
#include <memory>

// UI要素を階層的にグループ化するためのパネルクラス
class UIPanel : public UIElement
{
public:
    UIPanel();
    virtual ~UIPanel();

    void AddChild(std::unique_ptr<UIElement> child);
    const std::vector<std::unique_ptr<UIElement>>& GetChildren() const;

    // --- UIElementの仮想関数をオーバーライド ---
    void UpdateLogic(float deltaTime) override; // ★ 修正点: deltaTime引数を追加
    void UpdateInteraction() override;          // ★ 追加: Interactionの更新もオーバーライド
    void Draw() const override;
    void SetVisible(bool visible) override;

protected:
    std::vector<std::unique_ptr<UIElement>> m_children;

    // ★ 改善案: Zオーダーが変更されたかを示すフラグ
    bool m_isZOrderDirty = false;
};
