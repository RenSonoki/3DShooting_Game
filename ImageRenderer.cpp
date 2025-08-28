//#include "ImageRenderer.h"
//#include "UIElement.h" // オーナーの情報を取得するために必要
//
//ImageRenderer::ImageRenderer(const std::wstring& path)
//{
//    // 内部のSpriteRendererに画像の読み込みを依頼
//    m_sprite.Load(path);
//}
//
//// 【修正点】引数を const UIElement* owner に変更
//void ImageRenderer::Draw(const UIElement* owner) const
//{
//    // オーナーがいない、または非表示なら描画しない
//    if (!owner || !owner->IsVisible())
//    {
//        return;
//    }
//
//    // オーナーからTransform情報を取得
//    const Transform2D& transform = owner->GetTransform();
//
//    // 内部のSpriteRendererに、取得したTransformと自身が持つ色を渡して描画を依頼
//    m_sprite.Draw(transform, true, m_color);
//}
//
//void ImageRenderer::SetColor(unsigned int color)
//{
//    m_color = color;
//}
//
//unsigned int ImageRenderer::GetColor() const
//{
//    return m_color;
//}
//
//int ImageRenderer::GetOriginalWidth() const
//{
//    return m_sprite.GetOriginalWidth();
//}
//
//int ImageRenderer::GetOriginalHeight() const
//{
//    return m_sprite.GetOriginalHeight();
//}