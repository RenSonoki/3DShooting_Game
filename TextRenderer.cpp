#include "TextRenderer.h"
#include "UIText.h" // オーナーをUITextにキャストしてテキスト情報を取得するため
#include "DxLib.h"

// RAIIを活用してフォントサイズ設定を管理するヘルパークラス
class ScopedFontSize
{
private:
    int m_oldFontSize;
public:
    // コンストラクタで新しいフォントサイズを設定
    explicit ScopedFontSize(int newSize)
    {
        m_oldFontSize = GetFontSize(); // 現在のフォントサイズを記憶
        SetFontSize(newSize);          // 新しいサイズに変更
    }
    // デストラクタで記憶しておいた古いフォントサイズに戻す
    ~ScopedFontSize()
    {
        SetFontSize(m_oldFontSize);
    }
    // コピーとムーブを禁止
    ScopedFontSize(const ScopedFontSize&) = delete;
    ScopedFontSize& operator=(const ScopedFontSize&) = delete;
};


void TextRenderer::Draw(const UIElement* owner) const
{
    // オーナーをUITextにダウンキャスト
    const auto* textElement = static_cast<const UIText*>(owner);
    if (!textElement) return;

    // RAIIクラスを使い、この関数のスコープ内でのみフォントサイズを変更する
    const ScopedFontSize scopedFont(textElement->GetFontSizePx());

    // 描画に必要な情報をオーナーから取得
    const auto& transform = textElement->GetTransform();
    const auto& text = textElement->GetText();
    const unsigned int color = textElement->GetColor();
    const VECTOR2 pos = transform.GetPosition();

    // DXライブラリの描画関数を呼び出し
    DrawString(static_cast<int>(pos.x), static_cast<int>(pos.y), text.c_str(), color);
}