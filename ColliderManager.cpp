#include "ColliderManager.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <algorithm>
#include <vector>
#include <set>
#include <unordered_set>

ColliderManager& ColliderManager::GetInstance()
{
    static ColliderManager instance;
    return instance;
}

void ColliderManager::Init(float worldWidth, float worldDepth, float cellSize)
{
    m_cellSize = (cellSize > 1e-6f) ? cellSize : 1.0f;
    m_gridCols = static_cast<int>(std::ceil(worldWidth / m_cellSize));
    m_gridRows = static_cast<int>(std::ceil(worldDepth / m_cellSize));
    Clear();
}

void ColliderManager::Register(const std::shared_ptr<ICollisionComponent>& collider)
{
    if (collider)
    {
        m_colliders.push_back(collider);
    }
}

void ColliderManager::Unregister(const std::shared_ptr<ICollisionComponent>& collider)
{
    if (collider)
    {
        m_pendingRemoval.push_back(collider);
    }
}

void ColliderManager::RegisterCollisionCheck(CollisionPair pair, CollisionFunc func)
{
    m_collisionCheckers[pair] = func;
}

void ColliderManager::CheckAllCollisions()
{
    ApplyPendingRemovals();
    UpdateGrid();

    std::set<std::pair<const ICollisionComponent*, const ICollisionComponent*>> checkedPairs;

    for (const auto& gridCellPair : m_grid)
    {
        const auto& currentCell = gridCellPair.second;
        // (ここでは簡易化のため同一セル内のみをチェック)
        for (const auto& a : currentCell)
        {
            if (!a || !a->IsActive()) continue;
            for (const auto& b : currentCell)
            {
                if (!b || !b->IsActive() || a == b) continue;

                auto pairToTest = (a.get() < b.get()) ? std::make_pair(a.get(), b.get()) : std::make_pair(b.get(), a.get());
                if (checkedPairs.count(pairToTest)) continue;

                CollisionPair typePair(a->GetShapeType(), b->GetShapeType());
                auto checkerIt = m_collisionCheckers.find(typePair);
                if (checkerIt != m_collisionCheckers.end())
                {
                    CollisionInfo info;
                    if (checkerIt->second(*a, *b, info))
                    {
                        ResolveCollision(*a, *b, info);
                        a->TriggerCollision(b->GetOwner());
                        b->TriggerCollision(a->GetOwner());
                    }
                }
                checkedPairs.insert(pairToTest);
            }
        }
    }
}

// ★ RigidbodyComponentへの依存を削除した、シンプルな衝突解決
void ColliderManager::ResolveCollision(ICollisionComponent& a, ICollisionComponent& b, const CollisionInfo& info)
{
    auto ownerA = a.GetOwner();
    auto ownerB = b.GetOwner();
    if (!ownerA || !ownerB) return;

    auto transformA = ownerA->GetComponent<TransformComponent>();
    auto transformB = ownerB->GetComponent<TransformComponent>();
    if (!transformA || !transformB) return;

    // 両者を半分ずつ押し出す
    VECTOR push = VScale(info.normal, info.depth * 0.5f);
    transformA->SetPosition(VAdd(transformA->GetPosition(), push));
    transformB->SetPosition(VSub(transformB->GetPosition(), push));
}

void ColliderManager::Clear()
{
    m_colliders.clear();
    m_pendingRemoval.clear();
    m_grid.clear();
}

void ColliderManager::ApplyPendingRemovals()
{
    if (m_pendingRemoval.empty()) return;

    std::unordered_set<std::shared_ptr<ICollisionComponent>> removalSet(m_pendingRemoval.begin(), m_pendingRemoval.end());
    std::erase_if(m_colliders, [&removalSet](const auto& c) {
        return removalSet.count(c) > 0;
        });

    m_pendingRemoval.clear();
}

void ColliderManager::UpdateGrid()
{
    m_grid.clear();
    for (const auto& collider : m_colliders)
    {
        if (collider && collider->IsActive())
        {
            int index = GetGridIndex(collider->GetCenter());
            m_grid[index].push_back(collider);
        }
    }
}

int ColliderManager::GetGridIndex(const VECTOR& position) const
{
    if (m_cellSize < 1e-6f) return 0;
    int x = static_cast<int>(position.x / m_cellSize);
    int z = static_cast<int>(position.z / m_cellSize);

    x = std::clamp(x, 0, m_gridCols - 1);
    z = std::clamp(z, 0, m_gridRows - 1);

    return x + z * m_gridCols;
}