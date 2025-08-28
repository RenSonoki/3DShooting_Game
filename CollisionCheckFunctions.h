#pragma once
#include "CollisionSystemTypes.h" // CollisionInfo や CollisionFunc の定義

// --- 衝突判定関数群 ---

// 球 vs 球
bool CheckSphereToSphere(const ICollisionComponent& a, const ICollisionComponent& b, CollisionInfo& outInfo);

// 球 vs カプセル
bool CheckSphereToCapsule(const ICollisionComponent& a, const ICollisionComponent& b, CollisionInfo& outInfo);

// カプセル vs カプセル
bool CheckCapsuleToCapsule(const ICollisionComponent& a, const ICollisionComponent& b, CollisionInfo& outInfo);