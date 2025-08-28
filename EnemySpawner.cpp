#include "EnemySpawner.h"
#include "EnemyEntity.h"
#include "EntitySystem.h"
#include "EnemyPrototype.h"
#include "TransformComponent.h"
#include "Random.h"
#include <DxLib.h> // DX_PI_Fのため
#include <cassert>

EnemySpawner::EnemySpawner(
    EntitySystem* entitySystem,
    std::shared_ptr<EnemyPrototype> prototype,
    std::weak_ptr<TransformComponent> playerTransform)
    // メンバー変数を初期化
    : m_entitySystem(entitySystem)
    , m_prototype(prototype)
    , m_playerTransform(playerTransform)
    , m_spawnInterval(3.0f) // デフォルトの出現間隔
    , m_spawnDistance(60.0f) // デフォルトの出現距離
    , m_timer(0.0f)
{
    // 依存関係が正しく渡されているかデバッグ時にチェック
    assert(m_entitySystem != nullptr && "EnemySpawner requires an EntitySystem.");
    assert(m_prototype != nullptr && "EnemySpawner requires an EnemyPrototype.");
}

void EnemySpawner::Update(float deltaTime)
{
    m_timer += deltaTime;
    if (m_timer >= m_spawnInterval)
    {
        m_timer -= m_spawnInterval; // タイマーをリセット
        Spawn();
    }
}

void EnemySpawner::SetSpawnInterval(float seconds) { m_spawnInterval = seconds; }
void EnemySpawner::SetSpawnDistance(float distance) { m_spawnDistance = distance; }

// private
void EnemySpawner::Spawn()
{
    // プレイヤーのTransformを安全に取得
    auto playerTr = m_playerTransform.lock();
    if (!playerTr) return; // プレイヤーがいない場合は何もしない

    // PlaySceneにあったロジックをそのまま移植
    float angle = Random::GetInstance().GetFloat(0.f, DX_PI_F * 2.0f);
    VECTOR offset = VGet(sinf(angle) * m_spawnDistance, 0, cosf(angle) * m_spawnDistance);
    VECTOR spawnPos = VAdd(playerTr->GetPosition(), offset);

    // プロトタイプから敵を複製し、EntitySystemに追加
    auto enemy = m_prototype->Clone(spawnPos, playerTr);
    m_entitySystem->AddEntity(enemy);
}