#include "UIText.h"
#include "TextRenderer.h" // 自身に装着する部品
#include <memory>

UIText::UIText(const std::wstring& text, unsigned int color, int fontSize)
    : m_text(text)
    , m_color(color)
    , m_fontSize(fontSize)
{
    // 自身の「描画」振る舞いを担当する部品を生成し、装着する
    auto renderer = std::make_unique<TextRenderer>();
    SetRenderer(std::move(renderer));
}

void UIText::SetText(const std::wstring& text) { m_text = text; }
void UIText::SetColor(unsigned int color) { m_color = color; }
void UIText::SetFontSizePx(int size) { m_fontSize = size; }
const std::wstring& UIText::GetText() const { return m_text; }
unsigned int UIText::GetColor() const { return m_color; }
int UIText::GetFontSizePx() const { return m_fontSize; }