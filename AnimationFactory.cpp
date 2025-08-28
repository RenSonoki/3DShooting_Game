#include "AnimationFactory.h"

int AnimationFactory::GetIndex(int modelHandle, const std::wstring& animName)
{
    Key key{ modelHandle, animName };

    auto it = m_cache.find(key);
    if (it != m_cache.end()) {
        return it->second;
    }

    int index = MV1GetAnimIndex(modelHandle, animName.c_str());
    if (index != -1) {
        m_cache[key] = index;
    }

    return index; // Ž¸”sŽž‚Í -1
}

void AnimationFactory::Clear()
{
    m_cache.clear();
}