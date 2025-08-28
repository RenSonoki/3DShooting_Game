#pragma once
#include <vector>
#include <memory>

class UIElement; // UIElementの前方宣言

// UIElementの階層を管理し、更新・描画を行うUI専門のシステム
class UISystem
{
public:
    UISystem();
    ~UISystem();

    UISystem(const UISystem&) = delete;
    UISystem& operator=(const UISystem&) = delete;

    void AddElement(std::unique_ptr<UIElement> element);
    void Update(float deltaTime);
    void Draw() const;
    void Clear();
    void SetLayerDepth(int depth);
    int GetLayerDepth() const;

private:
    void CollectRenderables(UIElement* element, std::vector<UIElement*>& renderList) const;

    std::vector<std::unique_ptr<UIElement>> m_rootElements;
    int m_layerDepth = 0;
};