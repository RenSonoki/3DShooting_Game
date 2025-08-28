#pragma once
#include <functional> // ★ std::hash のためにインクルード

// プロジェクトに存在するコンポーネントの種類をすべて列挙します。
enum class ComponentID
{
    // ... (内容は変更なし)
    Transform,
    SpriteRenderer,
    ModelRenderer,
    PlayerController,
    AIController,
    Rigidbody,
    Collider,
    Camera,
    CameraController,
    Animator,
    Move,
    Life,
    Homing,
    Health,
    Shooting,
    Sphere,
	Capsule,
    ThirdPersonCamera
};


// --- std::hash for ComponentID ---
// std::unordered_mapがComponentIDをキーとして使えるように、ハッシュ関数を定義します
namespace std
{
    template <>
    struct hash<ComponentID>
    {
        size_t operator()(const ComponentID& id) const
        {
            // enum class の実体である整数値を、そのままハッシュ値として利用します
            return hash<std::underlying_type_t<ComponentID>>()(
                static_cast<std::underlying_type_t<ComponentID>>(id)
                );
        }
    };
}