#include "ResourceManager.h"
#include <functional> // std::applyのため

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

ResourceManager::~ResourceManager()
{
    ClearAll();
}

void ResourceManager::ClearAll()
{
    std::lock_guard<std::mutex> lock(m_resourceMutex);

    // C++17のstd::applyを使い、tuple内の全てのCacheに対してClear()を呼び出す
    std::apply(
        [](auto&... cache) {
            (cache.Clear(), ...);
        },
        m_allCaches
    );
}