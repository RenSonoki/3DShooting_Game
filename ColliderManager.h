#pragma once
#include "ICollisionComponent.h"
#include "CollisionSystemTypes.h"
#include <vector>
#include <memory>
#include <map>

class ColliderManager
{
public:
    // シングルトンインスタンスを取得
    static ColliderManager& GetInstance();

    // グリッド空間の初期化
    void Init(float worldWidth, float worldDepth, float cellSize);

    // コライダーの登録/解除
    void Register(const std::shared_ptr<ICollisionComponent>& collider);
    void Unregister(const std::shared_ptr<ICollisionComponent>& collider);

    // 形状ペアに対応する衝突判定関数を登録
    void RegisterCollisionCheck(CollisionPair pair, CollisionFunc func);

    // 毎フレームの衝突判定処理
    void CheckAllCollisions();

    // 管理下の全情報をクリア
    void Clear();

private:
    // シングルトンパターンの定石
    ColliderManager() = default;
    ~ColliderManager() = default;
    ColliderManager(const ColliderManager&) = delete;
    ColliderManager& operator=(const ColliderManager&) = delete;

    // ヘルパー関数
    void ApplyPendingRemovals();
    void UpdateGrid();
    int GetGridIndex(const VECTOR& position) const;
    void ResolveCollision(ICollisionComponent& a, ICollisionComponent& b, const CollisionInfo& info);

    // メンバー変数
    std::vector<std::shared_ptr<ICollisionComponent>> m_colliders;
    std::vector<std::shared_ptr<ICollisionComponent>> m_pendingRemoval;
    std::map<int, std::vector<std::shared_ptr<ICollisionComponent>>> m_grid;
    std::map<CollisionPair, CollisionFunc> m_collisionCheckers;

    float m_cellSize;
    int m_gridCols;
    int m_gridRows;
};