//#include "SpriteComponent.h"
//#include "Transform2D.h" // Drawに渡すために必要
//
//SpriteComponent::SpriteComponent(const std::wstring& imagePath)
//{
//    // コンストラクタで、内包するSpriteRendererに画像をロードさせる
//    m_renderer.Load(imagePath);
//}
//
//void SpriteComponent::Draw(int targetScreen)
//{
//    // 自身のTransform情報(GetTransform())と可視状態(IsVisible())を、
//    // 内包するSpriteRendererに渡して、描画処理を「依頼」する。
//    // (targetScreenは、もしSpriteRenderer側で対応させるなら渡す)
//    m_renderer.Draw(GetTransform(), IsVisible());
//}