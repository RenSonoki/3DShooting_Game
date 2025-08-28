#pragma once
#include "UIAnimator.h"

/**
 * @class LogoWaveAnimator
 * @brief UI要素をサイン波で上下に揺らす具体的なアニメーター
 */
class LogoWaveAnimator : public UIAnimator
{
public:
    LogoWaveAnimator();

    // IUIAnimatorインターフェースの実装
    void Update(const UIElement* owner, Transform2D& transform, float deltaTime) override;

    // アニメーションのパラメータを設定するメソッド
    void Setup(float baseY, float amplitude, float frequency);

private:
    // アニメーション用パラメータ
    float m_baseY;
    float m_amplitude;
    float m_frequency;

    // 内部タイマー
    float m_time;
};