#pragma once
#include <string>
#include <memory>
#include <functional>
#include "DxLib.h"

class EnemyEntity;
class TransformComponent;

class EnemyPrototype
{
public:
    EnemyPrototype();

    void SetModelPath(const std::wstring& path);
    void SetSpeed(float speed);
    void SetCollisionRadius(float radius);
    void SetScore(int score);
    void SetOnDestroyCallback(std::function<void(int)> callback);

    std::shared_ptr<EnemyEntity> Clone(const VECTOR& spawnPos, std::weak_ptr<TransformComponent> targetTransform) const;

private:
    std::wstring m_modelPath;
    float m_speed;
    float m_radius;
    int m_score;
    std::function<void(int)> m_onDestroyCallback;
};