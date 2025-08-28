#pragma once
#include "ResourceTraits.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <tuple>
#include <Windows.h>

// 特定の１種類のリソースを管理する汎用キャッシュクラス
template<typename Tag>
class Cache
{
public:
    using Traits = ResourceTraits<Tag>;
    using HandleType = typename Traits::HandleType;

    // キャッシュからリソースを取得（なければロード）する
    HandleType Get(const std::wstring& path)
    {
        // NOTE: この実装ではResourceManager側で一括ロックしていますが、
        //       ここにMutexを持たせれば、より粒度の細かいロックも可能です。
        auto it = m_data.find(path);
        if (it != m_data.end())
        {
            return it->second;
        }

        HandleType handle = Traits::Load(path);
        if (handle != -1)
        {
            m_data[path] = handle;
        }
        else
        {
            OutputDebugStringW((L"[Resource Load Failed] " + path + L"\n").c_str());
        }
        return handle;
    }

    // このキャッシュが保持する全てのリソースを解放する
    void Clear()
    {
        for (auto const& [path, handle] : m_data)
        {
            Traits::Unload(handle);
        }
        m_data.clear();
    }
private:
    std::unordered_map<std::wstring, HandleType> m_data;
};


// 汎用リソースマネージャー (Singleton)
class ResourceManager
{
public:
    static ResourceManager& Instance();

    // テンプレート特殊化が不要になり、単一の汎用メソッドになる
    template<typename Tag>
    typename ResourceTraits<Tag>::HandleType Get(const std::wstring& path);

    void ClearAll();

private:
    ResourceManager() = default;
    ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // --- 管理するリソースの種類をここで定義 ---
    // NOTE: 新しいリソースを追加する場合、このtupleに Cache<NewTag> を追加するだけでOK！
    using AllCaches = std::tuple<
        Cache<ModelTag>,
        Cache<ImageTag>,
        Cache<SoundTag>
        // Cache<EffectTag>,
        // Cache<FontTag> ...
    >;

    AllCaches m_allCaches;
    std::mutex m_resourceMutex;
};


// --- テンプレート実装（ヘッダに記述） ---
template<typename Tag>
typename ResourceTraits<Tag>::HandleType ResourceManager::Get(const std::wstring& path)
{
    std::lock_guard<std::mutex> lock(m_resourceMutex);

    // tupleから、Tagに対応するCacheを取り出してGetを呼ぶ
    return std::get<Cache<Tag>>(m_allCaches).Get(path);
}