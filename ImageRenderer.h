// SimpleSpriteRendererに移管済み

//#pragma once
//#include "UIRenderer.h"     // 私たちが定義したUIRendererインターフェース
//#include "SpriteRenderer.h"   // あなたが作った描画ユーティリティ
//#include "Color.h"      // あなたが作った色ユーティリティ
//#include <string>
//
//// 画像描画という「振る舞い」を具体的に実装するクラス
//class ImageRenderer : public UIRenderer
//{
//public:
//    // コンストラクタで画像のパスを受け取る
//    explicit ImageRenderer(const std::wstring& path);
//    virtual ~ImageRenderer() = default;
//
//    // --- UIRendererインターフェースの約束事を実装 ---
//    // 【修正点】引数を const UIElement* owner に変更
//    void Draw(const UIElement* owner) const override;
//
//    // --- この振る舞い固有の操作 ---
//    void SetColor(unsigned int color);
//    unsigned int GetColor() const;
//    int GetOriginalWidth() const;
//    int GetOriginalHeight() const;
//
//private:
//    // SpriteRendererを部品として所有（コンポジション）
//    SpriteRenderer m_sprite;
//
//    // この振る舞い固有のデータ
//    unsigned int m_color = ColorUtil::White;
//};