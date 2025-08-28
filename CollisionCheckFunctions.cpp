#include "CollisionCheckFunctions.h"
#include "SphereCollisionComponent.h"
#include "CapsuleCollisionComponent.h"
// #include "BoxCollisionComponent.h" // ボックス関連の判定を実装する際に必要
#include <cmath>
#include <algorithm>
#include <limits>

// --- ヘルパー関数 ---

// 点Pから線分ABへの最近接点を計算する
static VECTOR ClosestPointOnLineSegment(const VECTOR& p, const VECTOR& a, const VECTOR& b)
{
    const VECTOR ab = VSub(b, a);
    const float abLenSq = VSquareSize(ab);
    if (abLenSq < 1e-12f) return a; // 線分がほぼ点の場合のエッジケース処理

    // abベクトルに対するp-aベクトルの射影を計算し、パラメータtを求める
    const float t = VDot(VSub(p, a), ab) / abLenSq;

    // tを0.0～1.0の範囲にクランプし、線分上の点を求める
    return VAdd(a, VScale(ab, std::clamp(t, 0.0f, 1.0f)));
}

// 2つの線分間の最近接点ペアを計算するヘルパー関数
static std::pair<VECTOR, VECTOR> ClosestPointsBetweenLineSegments(
    const VECTOR& p1, const VECTOR& p2, const VECTOR& q1, const VECTOR& q2)
{
    VECTOR d1 = VSub(p2, p1);
    VECTOR d2 = VSub(q2, q1);
    VECTOR r = VSub(p1, q1);
    float a = VDot(d1, d1), e = VDot(d2, d2), f = VDot(d2, r);
    float s = 0.0f, t = 0.0f;

    if (a <= 1e-6f && e <= 1e-6f) { return { p1, q1 }; }
    if (a <= 1e-6f) { t = std::clamp(f / e, 0.0f, 1.0f); }
    else {
        float c = VDot(d1, r);
        if (e <= 1e-6f) { s = std::clamp(-c / a, 0.0f, 1.0f); }
        else {
            float b = VDot(d1, d2);
            float denom = a * e - b * b;
            if (denom > 1e-6f) { s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f); }
            t = (b * s + f) / e;
            if (t < 0.0f) { t = 0.0f; s = std::clamp(-c / a, 0.0f, 1.0f); }
            else if (t > 1.0f) { t = 1.0f; s = std::clamp((b - c) / a, 0.0f, 1.0f); }
        }
    }
    return { VAdd(p1, VScale(d1, s)), VAdd(q1, VScale(d2, t)) };
}


// --- 衝突判定関数の実装 ---

// 球 vs 球
bool CheckSphereToSphere(const ICollisionComponent& a, const ICollisionComponent& b, CollisionInfo& outInfo)
{
    const auto& sphereA = static_cast<const SphereCollisionComponent&>(a);
    const auto& sphereB = static_cast<const SphereCollisionComponent&>(b);

    VECTOR vecAB = VSub(sphereA.GetCenter(), sphereB.GetCenter());
    const float distSq = VSquareSize(vecAB);
    const float radiusSum = sphereA.GetRadius() + sphereB.GetRadius();

    if (distSq > radiusSum * radiusSum) return false; // 衝突していない

    const float dist = sqrtf(distSq);
    outInfo.depth = radiusSum - dist; // めり込み量
    // 押し出す方向（BからAへ）
    outInfo.normal = (dist > 1e-6f) ? VScale(vecAB, 1.0f / dist) : VGet(0, 1, 0);
    return true;
}

// カプセル vs 球
bool CheckCapsuleToSphere(const ICollisionComponent& a, const ICollisionComponent& b, CollisionInfo& outInfo)
{
    const auto& capsule = static_cast<const CapsuleCollisionComponent&>(a);
    const auto& sphere = static_cast<const SphereCollisionComponent&>(b);

    // カプセルの中心線上で、球の中心に最も近い点を求める
    auto capsuleSegment = capsule.GetWorldLineSegment();
    VECTOR closestPointOnSegment = ClosestPointOnLineSegment(sphere.GetCenter(), capsuleSegment.first, capsuleSegment.second);

    // その最近接点と、球の中心との間で、単純な「球 vs 球」の判定を行う
    VECTOR vec = VSub(sphere.GetCenter(), closestPointOnSegment);
    float distSq = VSquareSize(vec);
    float totalRadius = capsule.GetRadius() + sphere.GetRadius();

    if (distSq > totalRadius * totalRadius) return false;

    float dist = sqrtf(distSq);
    outInfo.depth = totalRadius - dist;
    outInfo.normal = (dist > 1e-6f) ? VScale(vec, 1.0f / dist) : VGet(0, 1, 0);
    return true;
}

// 球 vs カプセルは、引数を入れ替えて上記関数を呼ぶだけ
bool CheckSphereToCapsule(const ICollisionComponent& a, const ICollisionComponent& b, CollisionInfo& outInfo)
{
    if (CheckCapsuleToSphere(b, a, outInfo))
    {
        // 法線の向きを反転させるのを忘れない
        outInfo.normal = VScale(outInfo.normal, -1.0f);
        return true;
    }
    return false;
}

// カプセル vs カプセル
bool CheckCapsuleToCapsule(const ICollisionComponent& a, const ICollisionComponent& b, CollisionInfo& outInfo)
{
    const auto& capsuleA = static_cast<const CapsuleCollisionComponent&>(a);
    const auto& capsuleB = static_cast<const CapsuleCollisionComponent&>(b);

    // 2つのカプセルの中心線を取得
    auto segmentA = capsuleA.GetWorldLineSegment();
    auto segmentB = capsuleB.GetWorldLineSegment();

    // 2つの線分間の最近接点ペアを求める
    auto closestPoints = ClosestPointsBetweenLineSegments(segmentA.first, segmentA.second, segmentB.first, segmentB.second);

    // 最近接点間の距離で「球 vs 球」の判定を行う
    VECTOR vec = VSub(closestPoints.first, closestPoints.second);
    float distSq = VSquareSize(vec);
    float totalRadius = capsuleA.GetRadius() + capsuleB.GetRadius();

    if (distSq > totalRadius * totalRadius) return false;

    float dist = sqrtf(distSq);
    outInfo.depth = totalRadius - dist;
    outInfo.normal = (dist > 1e-6f) ? VScale(vec, 1.0f / dist) : VGet(0, 1, 0);
    return true;
}