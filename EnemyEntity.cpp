#include "EnemyEntity.h"
#include "HomingMoveComponent.h"
#include <utility> // std::move

EnemyEntity::EnemyEntity()
{
    SetTag(L"Enemy");
}

void EnemyEntity::SetTarget(std::shared_ptr<TransformComponent> target)
{
    if (auto homingMove = GetComponent<HomingMoveComponent>())
    {
        homingMove->SetTarget(target);
    }
}

void EnemyEntity::SetScore(int score) { m_score = score; }
int EnemyEntity::GetScore() const { return m_score; }

void EnemyEntity::SetOnDestroyCallback(std::function<void(int)> callback)
{
    m_onDestroyCallback = std::move(callback);
}