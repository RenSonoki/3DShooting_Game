#include <algorithm>
#include <unordered_set>

#include "Entity.h"
#include "EntitySystem.h"


void EntitySystem::AddEntity(std::shared_ptr<Entity> entity)
{
    if (entity)
    {
        // 更新ループ中なら、後で追加するためにキューに入れる
        if (m_isUpdating)
        {
            m_pendingAdd.push_back(entity);
        }
        else // 更新中でなければ即座に追加し、Startを呼ぶ
        {
            m_entities.push_back(entity);
            entity->Start(); // NOTE: すぐにStartを呼ぶ
        }
    }
}

void EntitySystem::RemoveEntity(std::shared_ptr<Entity> entity)
{
    if (entity)
    {
        // 削除キューには生のポインタを入れておく
        m_pendingRemoval.push_back(entity.get());
    }
}

size_t EntitySystem::GetEntityCount() const
{
    return m_entities.size();
}

const std::vector<std::shared_ptr<Entity>>& EntitySystem::GetEntities() const
{
    return m_entities;
}

void EntitySystem::StartAll()
{
    for (auto& e : m_entities)
    {
        if (e && e->IsActive())
        {
            e->Start();
        }
    }
}

void EntitySystem::UpdateAll(float deltaTime)
{
    m_isUpdating = true;
    for (auto& entity : m_entities)
    {
        if (entity && entity->IsActive())
        {
            entity->Update(deltaTime);
        }
    }
    m_isUpdating = false;

    ApplyPendingActions();
}

void EntitySystem::DrawAll() const
{
    for (const auto& entity : m_entities)
    {
        if (entity && entity->IsActive())
        {
            entity->Draw();
        }
    }
}

void EntitySystem::Clear()
{
    m_entities.clear();
    m_pendingAdd.clear();
    m_pendingRemoval.clear();
}


void EntitySystem::ApplyPendingActions()
{
    // --- 削除処理（より効率的な方法に修正） ---
    if (!m_pendingRemoval.empty())
    {
        // 削除対象のポインタを高速検索できるセットに入れる
        const std::unordered_set<Entity*> removalSet(m_pendingRemoval.begin(), m_pendingRemoval.end());

        // C++20の std::erase_if を使う
        std::erase_if(m_entities, [&removalSet](const auto& entity) {
            return removalSet.count(entity.get()) > 0;
            });

        m_pendingRemoval.clear();
    }

    // --- 追加処理 ---
    if (!m_pendingAdd.empty())
    {
        // 予約されていたエンティティをまとめて追加
        for (const auto& toAdd : m_pendingAdd)
        {
            m_entities.push_back(toAdd);
            // ★ 重要: 追加したエンティティのStartをすぐに呼び出す
            if (toAdd)
            {
                toAdd->Start();
            }
        }
        m_pendingAdd.clear();
    }
}