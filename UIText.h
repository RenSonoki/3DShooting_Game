#pragma once
#include "UIElement.h"
#include "Color.h" // 自作のColorUtilを使用
#include <string>

// UI用のテキスト要素を表すクラス
class UIText : public UIElement
{
public:
    explicit UIText(const std::wstring& text = L"", unsigned int color = ColorUtil::White, int fontSize = 20);
    virtual ~UIText() = default;

    // --- 状態設定 ---
    void SetText(const std::wstring& text);
    void SetColor(unsigned int color);
    void SetFontSizePx(int size);

    // --- 状態取得（Rendererが描画に使う） ---
    const std::wstring& GetText() const;
    unsigned int GetColor() const;
    int GetFontSizePx() const;

private:
    std::wstring m_text;
    unsigned int m_color;
    int m_fontSize; // Px単位
};