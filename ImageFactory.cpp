#include "ImageFactory.h"

int ImageFactory::Load(const std::wstring& path) {
    auto it = m_cache.find(path);
    if (it != m_cache.end()) {
        return it->second; // キャッシュがある場合は再利用
    }

    int handle = LoadGraph(path.c_str());
    if (handle != -1) {
        m_cache[path] = handle;
    }
    return handle;
}

void ImageFactory::Clear() {
    for (const auto& pair : m_cache) {
        DeleteGraph(pair.second);
    }
    m_cache.clear();
}