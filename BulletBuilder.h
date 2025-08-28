#pragma once
#include "DxLib.h"
#include <string>
#include <memory>

// 前方宣言
class BulletEntity;

// BulletEntityの構築を専門に行うビルダークラス
class BulletBuilder
{
public:
    BulletBuilder();

    // メソッドチェーンで設定を繋げられるように、自身の参照を返す
    BulletBuilder& SetPosition(const VECTOR& pos);
    BulletBuilder& SetDirection(const VECTOR& dir);
    BulletBuilder& SetSpeed(float speed);
    BulletBuilder& SetLifetime(float lifetime);
    BulletBuilder& SetModel(const std::wstring& modelPath);
    BulletBuilder& SetCollisionRadius(float radius);

    // 最終的に設定を元にBulletEntityを構築して返す
    std::shared_ptr<BulletEntity> Build() const;

private:
    // 構築に必要なパラメータを保持
    VECTOR m_pos;
    VECTOR m_dir;
    float m_speed;
    float m_lifetime;
    std::wstring m_modelPath;
    float m_collisionRadius;
};