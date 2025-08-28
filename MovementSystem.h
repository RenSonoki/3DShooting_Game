#pragma once
#include <vector>
#include <memory>

// 前方宣言
class Entity;

/**
 * @class MovementSystem
 * @brief TransformComponentの速度に基づいて、エンティティの位置を更新するシステム
 */
class MovementSystem
{
public:
    // EntitySystemが管理する全エンティティのリストを受け取って処理を行う
    void Update(const std::vector<std::shared_ptr<Entity>>& entities, float deltaTime);
};