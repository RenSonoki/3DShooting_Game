#pragma once
#include "UIElement.h"
#include <string>

// UI用の静的な画像要素を表すクラス
class UIImage : public UIElement
{
public:
    explicit UIImage(const std::wstring& imagePath);
    virtual ~UIImage() = default;

    // DrawやUpdateなどのロジックは、基底クラスであるUIElementが
    // 内部のRenderer部品に委譲してくれるため、このクラスに実装は不要。
};