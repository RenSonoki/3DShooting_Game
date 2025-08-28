#include "LogoWaveAnimator.h"
#include "UIElement.h"   // Updateの引数で使う
#include "Transform2D.h" // Transform2Dを操作するために必要
#include <cmath>         // sinf

LogoWaveAnimator::LogoWaveAnimator()
    : m_baseY(0.0f)
    , m_amplitude(10.0f)
    , m_frequency(1.0f)
    , m_time(0.0f)
{
}

void LogoWaveAnimator::Setup(float baseY, float amplitude, float frequency)
{
    m_baseY = baseY;
    m_amplitude = amplitude;
    m_frequency = frequency;
}

void LogoWaveAnimator::Update(const UIElement* owner, Transform2D& transform, float deltaTime)
{
    // 内部タイマーを更新
    m_time += deltaTime;

    // サイン波を計算
    const float angle = m_time * m_frequency;
    const float offsetY = std::sinf(angle) * m_amplitude;

    // 座標を更新
    VECTOR2 pos = transform.GetPosition();
    pos.y = m_baseY + offsetY;
    transform.SetPosition(pos);
}