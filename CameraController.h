// ThirdPersonCameraComponentに移管済み

//#pragma once
//
//#include "LogicComponent.h"
//#include <memory>
//#include <DxLib.h>
//
//// 前方宣言
//class CameraComponent;
//class TransformComponent;
//
//// カメラモードの種類の列挙型
//enum class CameraMode
//{
//    ThirdPerson,
//    FirstPerson,
//    Free // このコンポーネントでは制御しないモード
//};
//
//// カメラの追従やモード切替など、複雑なロジックを担うコンポーネント
//class CameraController : public LogicComponent
//{
//public:
//    // 制御対象のCameraComponentをコンストラクタで受け取る
//    explicit CameraController(std::shared_ptr<CameraComponent> camera);
//
//    // LogicComponentの純粋仮想関数をオーバーライド
//    void Update(float deltaTime) override;
//
//    // 追従するターゲット（プレイヤーなど）を設定
//    void SetTarget(std::shared_ptr<TransformComponent> target);
//
//    // カメラモードの切り替え
//    void SetMode(CameraMode mode);
//    CameraMode GetMode() const;
//
//    // TPSモードでのターゲットからの相対位置を設定
//    void SetOffset(const VECTOR& offset);
//
//    // カメラ追従の滑らかさを設定
//    void SetLerpSpeed(float speed);
//
//private:
//    std::shared_ptr<CameraComponent> m_camera;
//    std::shared_ptr<TransformComponent> m_target;
//
//    CameraMode m_mode = CameraMode::ThirdPerson;
//    VECTOR m_offset = VGet(0.0f, 5.0f, -15.0f); // デフォルトのTPSオフセット
//    float m_lerpSpeed = 10.0f; // 追従の強さ（大きいほど速く追従）
//
//    // 線形補間を行うヘルパー関数
//    static VECTOR Lerp(const VECTOR& from, const VECTOR& to, float t);
//};