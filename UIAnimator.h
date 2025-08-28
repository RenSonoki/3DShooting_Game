#pragma once

// 前方宣言
class UIElement;
class Transform2D;

// UI要素のアニメーション（ロジック更新）の振る舞いを定義するインターフェース
class UIAnimator
{
public:
    virtual ~UIAnimator() = default;

    /**
     * @brief アニメーションのロジックを更新します
     * @param owner このアニメーターを所有するUIElement
     * @param transform 操作対象のTransform2D
     * @param deltaTime 前フレームからの経過時間
     */
    virtual void Update(const UIElement* owner, Transform2D& transform, float deltaTime) = 0;
};