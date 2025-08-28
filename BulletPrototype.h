#pragma once
#include <string>
#include <memory>
#include "DxLib.h"

// 前方宣言
class Entity;
class BulletEntity;
class TransformComponent;

class BulletPrototype
{
public:
    // ★ デフォルトコンストラクタに変更
    BulletPrototype();

    // ★ セッターメソッドを追加
    void SetModel(const std::wstring& modelPath);
    void SetSpeed(float speed);
    void SetLifetime(float lifetime);

    // この設計図から新しい弾を複製する
    std::shared_ptr<BulletEntity> Clone(const VECTOR& position, const VECTOR& direction) const;

private:
    std::wstring m_modelPath;
    float m_speed;
    float m_lifetime;
};