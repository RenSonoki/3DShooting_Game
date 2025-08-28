#include "BulletBuilder.h"
#include "BulletEntity.h"

// 各コンポーネントのヘッダをインクルード
#include "TransformComponent.h"
#include "RenderModelComponent.h"
#include "MoveComponent.h"
#include "LifetimeComponent.h"
#include "SphereCollisionComponent.h"

// コンストラクタやセッターは変更なし
BulletBuilder::BulletBuilder()
    : m_pos({ 0.0f, 0.0f, 0.0f })
    , m_dir({ 0.0f, 0.0f, 1.0f })
    , m_speed(100.0f)
    , m_lifetime(5.0f)
    , m_modelPath(L"")
    , m_collisionRadius(0.5f)
{
}
BulletBuilder& BulletBuilder::SetPosition(const VECTOR& pos) { m_pos = pos; return *this; }
BulletBuilder& BulletBuilder::SetDirection(const VECTOR& dir) { m_dir = dir; return *this; }
BulletBuilder& BulletBuilder::SetSpeed(float speed) { m_speed = speed; return *this; }
BulletBuilder& BulletBuilder::SetLifetime(float lifetime) { m_lifetime = lifetime; return *this; }
BulletBuilder& BulletBuilder::SetModel(const std::wstring& modelPath) { m_modelPath = modelPath; return *this; }
BulletBuilder& BulletBuilder::SetCollisionRadius(float radius) { m_collisionRadius = radius; return *this; }


// ★★★ Buildメソッドを全面的に修正 ★★★
std::shared_ptr<BulletEntity> BulletBuilder::Build() const
{
    // ★ ここで make_shared を使うことで、AddComponentが呼ばれる前に
    //    エンティティが shared_ptr の管理下に入り、安全になります。
    auto bullet = std::make_shared<BulletEntity>();

    // 1. TransformComponentをEntityに作ってもらい、設定する
    auto transform = bullet->AddComponent<TransformComponent>();
    transform->SetPosition(m_pos);

    // 2. RenderModelComponentをEntityに作ってもらい、設定する
    auto renderer = bullet->AddComponent<RenderModelComponent>();
    renderer->SetModel(m_modelPath);

    // 3. MoveComponentをEntityに作ってもらい、設定する
    auto move = bullet->AddComponent<MoveComponent>();
    move->SetSpeed(m_speed);
    move->SetDirection(m_dir);

    // 4. LifetimeComponentをEntityに作ってもらい、設定する
    auto life = bullet->AddComponent<LifetimeComponent>();
    life->SetLifetime(m_lifetime);
    life->SetOnExpired([bullet_ptr = bullet.get()]() {
        bullet_ptr->SetActive(false);
        });

    // 5. SphereCollisionComponentをEntityに作ってもらい、設定する
    auto collider = bullet->AddComponent<SphereCollisionComponent>();
    collider->SetRadius(m_collisionRadius);

    // ラムダ式の引数を `Entity*` から `const std::shared_ptr<Entity>&` に変更
    collider->SetOnCollision([bullet_ptr = bullet.get()](const std::shared_ptr<Entity>& other) {
        if (other && other->GetTag() == L"Enemy") {
            bullet_ptr->SetActive(false);
        }
        });

    return bullet;
}