#pragma once
#include "DxLib.h"
#include <memory>

// 前方宣言
class CameraEntity;

/**
 * @class CameraBuilder
 * @brief CameraEntityの構築を専門に行うビルダークラス
 */
class CameraBuilder
{
public:
    CameraBuilder();

    // カメラの初期位置を設定します
    CameraBuilder& SetPosition(const VECTOR& pos);

    // 将来的にLookAtや視野角(FoV)などの設定メソッドを追加できます
    // CameraBuilder& SetLookAt(const VECTOR& target);
    // CameraBuilder& SetFieldOfView(float fov);

    // 最終的に設定を元にCameraEntityを構築して返します
    std::shared_ptr<CameraEntity> Build() const;

private:
    VECTOR m_position; // 構築時に設定するカメラの初期位置
};