#pragma once
#include <vector>
#include <memory>
#include <string>

#include "ComponentBase.h"

// ゲーム世界に存在する全てのオブジェクトの基底クラス(コンポーネントの入れ物)
class Entity : public std::enable_shared_from_this<Entity>
{
private:
    static int s_nextID;
    int m_entityID;

    bool m_active = true;
    bool m_started = false;
    std::wstring m_tag;

    // NOTE: 全てのコンポーネントを保持するリスト
    std::vector<std::shared_ptr<IComponent>> m_components;

public:
    Entity();
    virtual ~Entity() = default;

    // --- コンポーネント管理 ---

    template<typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args);

    template<typename T>
    std::shared_ptr<T> GetComponent() const;

    template<typename T>
    bool HasComponent() const;

    // --- ライフサイクル ---

    void Start();
    void Update(float deltaTime);
    void Draw() const;

    // --- ゲッター/セッター ---

    int GetID() const;

    bool IsActive() const;
    void SetActive(bool value);

    const std::wstring& GetTag() const;
    void SetTag(const std::wstring& tag);


};

// --- テンプレート関数の実装 ---

template<typename T, typename... Args>
std::shared_ptr<T> Entity::AddComponent(Args&&... args)
{
    // 新しいコンポーネントを生成
    auto newComponent = std::make_shared<T>(std::forward<Args>(args)...);

    // IComponentへのポインタにキャスト
    auto componentBase = std::static_pointer_cast<IComponent>(newComponent);

    // 所有者を設定
    componentBase->SetOwner(shared_from_this());

    // リストにのみ追加
    m_components.push_back(componentBase);

    // もしEntityがすでにStart済みなら、新しいコンポーネントのStartも即座に呼ぶ
    if (m_started)
    {
        newComponent->Start();
    }

    return newComponent;
}

template<typename T>
std::shared_ptr<T> Entity::GetComponent() const
{
    // 保持している全コンポーネントをループ
    for (const auto& comp : m_components)
    {
        // dynamic_pointer_cast を使って、安全なダウンキャストを試みる
        auto casted = std::dynamic_pointer_cast<T>(comp);
        if (casted)
        {
            // キャストに成功したら、それが目的のコンポーネント
            return casted;
        }
    }
    // 見つからなければ nullptr を返す
    return nullptr;
}

template<typename T>
bool Entity::HasComponent() const
{
    // GetComponentがnullptr以外を返せば、コンポーネントを持っているということ
    return GetComponent<T>() != nullptr;
}