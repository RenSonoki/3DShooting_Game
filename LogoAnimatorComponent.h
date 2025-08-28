// LogoWaveAnimatorに移管済み


//#pragma once
//
//#include "LogicComponent.h"
//#include <memory>
//
//// 前方宣言
//class Transform2D; 
//
//// UIに波のような上下動を与えるアニメーションコンポーネント
//class LogoAnimatorComponent : public LogicComponent
//{
//public:
//    LogoAnimatorComponent(float baseY, float amplitude, float frequency);
//
//    // ライフサイクルメソッド
//    void Start() override;
//    void Update(float deltaTime) override;
//
//    // パラメータ変更用のセッター
//    void SetBaseY(float baseY);
//    void SetAmplitude(float amplitude);
//    void SetFrequency(float frequency);
//
//private:
//    // shared_ptrではなく生ポインタでキャッシュする（所有はしないため）
//    Transform2D* m_transform = nullptr;
//
//    float m_baseY;
//    float m_amplitude;
//    float m_frequency;
//
//    // intではなくfloatのタイマーに変更
//    float m_time = 0.0f;
//};