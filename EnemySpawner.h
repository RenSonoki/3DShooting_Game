#pragma once
#include <memory>
#include <vector>

// 前方宣言
class EntitySystem;
class EnemyPrototype;
class TransformComponent;

/**
 * @class EnemySpawner
 * @brief 敵の出現ロジックを専門に担当するクラス
 */
class EnemySpawner
{
public:
    // 必要な依存関係をコンストラクタで受け取る
    EnemySpawner(
        EntitySystem* entitySystem,
        std::shared_ptr<EnemyPrototype> prototype,
        std::weak_ptr<TransformComponent> playerTransform
    );

    // 毎フレーム呼ばれる更新処理
    void Update(float deltaTime);

    // 外部からパラメータを変更できるようにセッターを用意
    void SetSpawnInterval(float seconds);
    void SetSpawnDistance(float distance);

private:
    // 実際の出現処理
    void Spawn();

    // 外部から与えられた「道具」へのポインタ
    EntitySystem* m_entitySystem;
    std::shared_ptr<EnemyPrototype> m_prototype;
    std::weak_ptr<TransformComponent> m_playerTransform;

    // 内部の状態変数
    float m_spawnInterval;
    float m_spawnDistance;
    float m_timer;
};