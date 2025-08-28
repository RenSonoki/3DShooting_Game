#include "SoundFactory.h"

int SoundFactory::Load(const std::wstring& path) {
    auto it = m_cache.find(path);
    if (it != m_cache.end()) {
        return it->second; // キャッシュ再利用
    }

    int handle = LoadSoundMem(path.c_str());
    if (handle != -1) {
        m_cache[path] = handle;
    }
    return handle;
}

void SoundFactory::Clear() {
    for (const auto& pair : m_cache) {
        DeleteSoundMem(pair.second); // サウンドハンドルの解放
    }
    m_cache.clear();
}