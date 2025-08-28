#include "UIManager.h"

// 【修正点】引数をunique_ptrに変更し、std::moveで所有権を移譲
void UIManager::AddSystem(const std::string& name, std::unique_ptr<UISystem> system)
{
    if (system)
    {
        m_systems[name] = std::move(system);
    }
}

// 【修正点】戻り値を生ポインタに変更
UISystem* UIManager::GetSystem(const std::string& name) const
{
    auto it = m_systems.find(name);
    // .get()でunique_ptrが管理する生ポインタを取得して返す
    return (it != m_systems.end()) ? it->second.get() : nullptr;
}

void UIManager::RemoveSystem(const std::string& name)
{
    m_systems.erase(name);
}

void UIManager::DrawAll() // 【修正点】引数を削除
{
    // 1. 描画対象のシステム（生ポインタ）を一時的なベクターに集める
    std::vector<UISystem*> systemsToDraw;
    systemsToDraw.reserve(m_systems.size());
    for (const auto& pair : m_systems)
    {
        if (pair.second)
        {
            systemsToDraw.push_back(pair.second.get());
        }
    }

    // 2. レイヤー深度に基づいてソートする
    std::sort(systemsToDraw.begin(), systemsToDraw.end(),
        [](const UISystem* a, const UISystem* b)
        {
            if (!a) return false;
            if (!b) return true;
            return a->GetLayerDepth() < b->GetLayerDepth();
        });

    // 3. ソートされた順序で描画する
    for (const auto* system : systemsToDraw)
    {
        system->Draw(); // UISystemのDraw()には引数は不要
    }
}

void UIManager::UpdateAll(float deltaTime)
{
    for (const auto& pair : m_systems)
    {
        if (pair.second)
        {
            pair.second->Update(deltaTime);
        }
    }
}

void UIManager::Clear()
{
    m_systems.clear();
}