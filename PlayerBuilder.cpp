#include "PlayerBuilder.h"
#include "PlayerEntity.h"

// 必要なコンポーネントのヘッダーをインクルード
#include "TransformComponent.h"
#include "RenderModelComponent.h"
#include "PlayerControllerComponent.h"
#include "BulletShooterComponent.h"
#include "HealthComponent.h"
#include "SphereCollisionComponent.h"
#include "ModelAnimatorComponent.h" // ★ アニメーターコンポーネントをインクルード
#include "ResourceManager.h" // Sound
#include "ResourceTraits.h"  //
#include <DxLib.h>           // PlaySoundMem

// ... PlayerBuilderのコンストラクタやセッターは変更ありません ...
PlayerBuilder::PlayerBuilder()
    : m_modelPath(L"")
    , m_bulletPrototype(nullptr)
    , m_entitySystem(nullptr)
    , m_maxHP(3)
    , m_initialInvincibility(1.5f)
    , m_collisionRadius(1.0f)
{
}
PlayerBuilder& PlayerBuilder::SetModel(const std::wstring& modelPath) { m_modelPath = modelPath; return *this; }
PlayerBuilder& PlayerBuilder::SetShooterInfo(std::shared_ptr<BulletPrototype> prototype, EntitySystem* entitySystem) { m_bulletPrototype = prototype; m_entitySystem = entitySystem; return *this; }
PlayerBuilder& PlayerBuilder::SetInitialHealth(int maxHP, float invincibilitySec) { m_maxHP = maxHP; m_initialInvincibility = invincibilitySec; return *this; }
PlayerBuilder& PlayerBuilder::SetCollisionRadius(float radius) { m_collisionRadius = radius; return *this; }


// --- 組み立て処理 ---
std::shared_ptr<PlayerEntity> PlayerBuilder::Build() const
{
    auto player = std::make_shared<PlayerEntity>();

    // --- 各コンポーネントを、正しい作法で生成・設定し、アタッチ ---
    player->AddComponent<TransformComponent>();
    player->AddComponent<PlayerControllerComponent>();

    auto renderer = player->AddComponent<RenderModelComponent>();
    renderer->SetModel(m_modelPath);

    // ★ ここに ModelAnimatorComponent を追加します
    auto animator = player->AddComponent<ModelAnimatorComponent>();
    if (animator)
    {
        // "idle"という名前で、待機モーションのファイルをアタッチ
        animator->AttachAnimation(L"idle", L"Assets/Animations/player_idle.vmd");
        // "walk"という名前で、歩行モーションのファイルをアタッチ
        animator->AttachAnimation(L"walk", L"Assets/Animations/player_walk.vmd");
        // 必要に応じて他のモーションも追加
        // animator->AttachAnimation(L"jump", L"Assets/Animations/player_jump.vmd");
    }

    auto shooter = player->AddComponent<BulletShooterComponent>();
    shooter->Setup(m_bulletPrototype, m_entitySystem);

    auto health = player->AddComponent<HealthComponent>();
    health->Setup(m_maxHP, m_initialInvincibility);
    health->SetOnDamageCallback([](int) {
        int handle = ResourceManager::Instance().Get<SoundTag>(L"Assets/SE/damage.wav");
        if (handle != -1) PlaySoundMem(handle, DX_PLAYTYPE_BACK);
        });
    health->SetOnDeathCallback([player_ptr = player.get()]() {
        player_ptr->SetActive(false);
        });

    auto collider = player->AddComponent<SphereCollisionComponent>();
    collider->SetRadius(m_collisionRadius);
    collider->SetOnCollision([player_ptr = player.get()](const std::shared_ptr<Entity>& other) {
        if (other && (other->GetTag() == L"Enemy" || other->GetTag() == L"EnemyBullet"))
        {
            if (auto healthComp = player_ptr->GetComponent<HealthComponent>())
            {
                healthComp->TakeDamage(1);
            }
        }
        });

    return player;
}