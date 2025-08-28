#include "PrototypeManager.h"
#include <stdexcept>
#include <utility> // std::move のため

PrototypeManager& PrototypeManager::Instance()
{
    static PrototypeManager instance;
    return instance;
}

// unique_ptrをstd::moveで受け取る
void PrototypeManager::Register(const std::wstring& id, std::unique_ptr<IEntityPrototype> prototype)
{
    if (prototype)
    {
        m_prototypes[id] = std::move(prototype);
    }
}

std::shared_ptr<Entity> PrototypeManager::Create(const std::wstring& id) const
{
    auto it = m_prototypes.find(id);
    if (it != m_prototypes.end())
    {
        // it->second は unique_ptr なので、.get() で生ポインタを取得してメソッドを呼ぶ
        return it->second->Clone();
    }
    throw std::runtime_error("PrototypeManager::Create: Unknown prototype ID: " + std::string(id.begin(), id.end()));
}

std::shared_ptr<Entity> PrototypeManager::CreateAt(const std::wstring& id, const VECTOR& pos) const
{
    auto it = m_prototypes.find(id);
    if (it != m_prototypes.end())
    {
        return it->second->CloneWithTransform(pos);
    }
    throw std::runtime_error("PrototypeManager::CreateAt: Unknown prototype ID: " + std::string(id.begin(), id.end()));
}

bool PrototypeManager::Has(const std::wstring& id) const
{
    return m_prototypes.count(id) > 0;
}

std::vector<std::wstring> PrototypeManager::GetAllIDs() const
{
    std::vector<std::wstring> result;
    result.reserve(m_prototypes.size()); // 事前にメモリ確保
    for (const auto& [id, _] : m_prototypes)
    {
        result.push_back(id);
    }
    return result;
}

void PrototypeManager::Clear()
{
    m_prototypes.clear();
}