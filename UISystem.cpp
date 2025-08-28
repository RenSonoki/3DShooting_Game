#include "UISystem.h"
#include "UIElement.h"
#include "UIPanel.h" // CollectRenderablesで使うため
#include <algorithm>
#include <DxLib.h>

// コンストラクタとデストラクタをここで定義
UISystem::UISystem() = default;
UISystem::~UISystem() = default;

void UISystem::AddElement(std::unique_ptr<UIElement> element)
{
    if (element)
    {
        m_rootElements.push_back(std::move(element));
    }
}

void UISystem::Update(float deltaTime)
{
    for (const auto& element : m_rootElements)
    {
        if (element && element->IsVisible())
        {
            element->UpdateLogic(deltaTime);
            element->UpdateInteraction();
        }
    }
}

void UISystem::Draw() const
{
    // 1. 描画対象をリストに集める (変更なし)
    std::vector<UIElement*> renderList;
    renderList.reserve(m_rootElements.size() * 2); // 事前に多めに確保
    for (const auto& element : m_rootElements)
    {
        if (element && element->IsVisible())
        {
            CollectRenderables(element.get(), renderList);
        }
    }
    // 2. Zオーダーでソート (変更なし)
    std::sort(renderList.begin(), renderList.end(),
        [](const UIElement* a, const UIElement* b) {
            return a->GetZOrder() < b->GetZOrder();
        });

    // ★★★ ここが唯一の描画設定の管理場所 ★★★

    // 3. UI描画の開始前に、描画設定をリセット＆設定
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    SetUseZBufferFlag(FALSE);
    SetWriteZBufferFlag(FALSE);

    // 4. ソートされた順序で、全UIを描画
    for (const auto* element : renderList)
    {
        element->Draw();
    }

    // 5. UI描画の終了後に、描画設定をデフォルトに戻す
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
    SetUseZBufferFlag(TRUE);
    SetWriteZBufferFlag(TRUE);
}

void UISystem::Clear()
{
    m_rootElements.clear();
}

void UISystem::SetLayerDepth(int depth) { m_layerDepth = depth; }
int UISystem::GetLayerDepth() const { return m_layerDepth; }

// private
void UISystem::CollectRenderables(UIElement* element, std::vector<UIElement*>& renderList) const
{
    if (!element || !element->IsVisible()) return;

    // 描画機能を持つ要素（Rendererがセットされているなど）のみをリストに追加
    if (element->HasRenderer())
    {
        renderList.push_back(element);
    }

    // もし要素がパネルなら、その子要素も再帰的に収集
    if (auto panel = dynamic_cast<UIPanel*>(element))
    {
        for (const auto& child : panel->GetChildren())
        {
            CollectRenderables(child.get(), renderList);
        }
    }
}