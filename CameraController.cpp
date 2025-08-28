//#include "CameraController.h"
//#include "CameraComponent.h"
//#include "TransformComponent.h"
//#include <DxLib.h>
//#include <cmath>
//
//CameraController::CameraController(std::shared_ptr<CameraComponent> camera)
//    // NOTE: std::moveは、C++11以降で導入された機能で、オブジェクトの所有権を移動するために使用されます。
//    : m_camera(std::move(camera))
//{
//}
//
//void CameraController::SetTarget(std::shared_ptr<TransformComponent> target)
//{
//    m_target = std::move(target);
//}
//
//void CameraController::SetMode(CameraMode mode)
//{
//    m_mode = mode;
//}
//
//CameraMode CameraController::GetMode() const
//{
//    return m_mode;
//}
//
//void CameraController::SetOffset(const VECTOR& offset)
//{
//    m_offset = offset;
//}
//
//void CameraController::SetLerpSpeed(float speed)
//{
//    m_lerpSpeed = speed;
//}
//
//VECTOR CameraController::Lerp(const VECTOR& from, const VECTOR& to, float t)
//{
//    // from から to へ t の割合だけ進んだベクトルを計算
//    return VAdd(from, VScale(VSub(to, from), t));
//}
//
//void CameraController::Update(float deltaTime)
//{
//    // 制御対象のカメラやターゲットがなければ何もしない
//    if (!m_camera) return;
//
//    // ターゲットが設定されていなければ原点を基準にする
//    VECTOR targetPos = m_target ? m_target->GetPosition() : VGet(0, 0, 0);
//    VECTOR desiredPos = targetPos; // カメラの最終的な目標位置
//
//    // カメラモードに応じて目標位置を決定
//    switch (m_mode)
//    {
//    case CameraMode::ThirdPerson:
//        desiredPos = VAdd(targetPos, m_offset);
//        break;
//
//    case CameraMode::FirstPerson:
//        desiredPos = targetPos;
//        break;
//
//    case CameraMode::Free:
//        return; // Freeモードではこのコンポーネントはカメラを制御しない
//    }
//
//    // フレームレートに依存しない、滑らかな移動のための補間率を計算
//    float t = 1.0f - expf(-m_lerpSpeed * deltaTime);
//
//    // 現在のカメラ位置から、計算した目標位置へ滑らかに移動させる
//    const VECTOR currentPos = m_camera->GetPosition();
//    const VECTOR newPos = Lerp(currentPos, desiredPos, t);
//
//    m_camera->SetPosition(newPos);
//    m_camera->SetTarget(targetPos);
//}