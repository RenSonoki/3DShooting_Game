#include "UIImage.h"
#include "SimpleSpriteRenderer.h" // 自身に装着する部品
#include <memory>

UIImage::UIImage(const std::wstring& imagePath)
{
    // 1. 自身の「描画」振る舞いを担当する部品を生成
    auto renderer = std::make_unique<SimpleSpriteRenderer>(imagePath);

    // 2. 基底クラスの機能を使って、生成した部品を自身に装着する
    //    これ以降、このUIImageインスタンスのDraw()が呼ばれると、
    //    自動的に上で生成したSimpleSpriteRendererのDraw()が実行される。
    SetRenderer(std::move(renderer));
}