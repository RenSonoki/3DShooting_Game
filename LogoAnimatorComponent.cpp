//#include "LogoAnimatorComponent.h"
//#include "Entity.h"
//#include "UIComponent.h" // UIComponentを探すためにインクルード
//#include "Transform2D.h"
//#include <cmath> // sinf
//#include <cassert>
//
//LogoAnimatorComponent::LogoAnimatorComponent(float baseY, float amplitude, float frequency)
//    : m_baseY(baseY)
//    , m_amplitude(amplitude)
//    , m_frequency(frequency)
//{
//}
//
//void LogoAnimatorComponent::Start()
//{
//    // オーナーエンティティからUIComponentを探し、そのTransformを取得する
//    auto uiComponent = GetOwner()->GetComponent<UIComponent>();
//    assert(uiComponent != nullptr && "LogoAnimatorComponent requires a UIComponent on the same entity.");
//
//    m_transform = &uiComponent->GetTransform();
//}
//
//void LogoAnimatorComponent::Update(float deltaTime)
//{
//    if (!m_transform) return;
//
//    // 時間を経過時間(deltaTime)で進める
//    m_time += deltaTime;
//
//    // sinの角度を計算
//    float angle = m_time * m_frequency;
//
//    // Y座標のオフセットを計算
//    float offsetY = std::sinf(angle) * m_amplitude;
//
//    VECTOR2 pos = m_transform->GetPosition();
//    pos.y = m_baseY + offsetY;
//    m_transform->SetPosition(pos);
//}
//
//void LogoAnimatorComponent::SetBaseY(float baseY)
//{
//    m_baseY = baseY;
//}
//
//void LogoAnimatorComponent::SetAmplitude(float amplitude)
//{
//    m_amplitude = amplitude;
//}
//
//void LogoAnimatorComponent::SetFrequency(float frequency)
//{
//    m_frequency = frequency;
//}