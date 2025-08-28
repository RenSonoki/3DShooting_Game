#include "CameraBuilder.h"
#include "CameraEntity.h"

// 必要なコンポーネントのヘッダーをインクルード
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ThirdPersonCameraComponent.h" // ★ CameraControllerの代わりにこちらをインクルード

CameraBuilder::CameraBuilder()
    : m_position({ 0.0f, 10.0f, -25.0f }) // デフォルトの初期位置
{
}

CameraBuilder& CameraBuilder::SetPosition(const VECTOR& pos)
{
    m_position = pos;
    return *this; // メソッドチェーンのため自身の参照を返す
}

std::shared_ptr<CameraEntity> CameraBuilder::Build() const
{
    // 1. まず、コンポーネントの入れ物となるEntityを生成
    auto cameraEntity = std::make_shared<CameraEntity>();

    // 2. EntityのAddComponent<T>機能を使って各コンポーネントを生成・追加

    // TransformComponentを追加し、位置を設定
    auto transform = cameraEntity->AddComponent<TransformComponent>();
    transform->SetPosition(m_position);

    // CameraComponentを追加（カメラとしての基本的な機能）
    cameraEntity->AddComponent<CameraComponent>();

    // ThirdPersonCameraComponentを追加（三人称視点のカメラ制御ロジック）
    cameraEntity->AddComponent<ThirdPersonCameraComponent>();

    // 3. 完成したエンティティを返す
    return cameraEntity;
}