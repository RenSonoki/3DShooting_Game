#pragma once
#include <functional>
#include <utility>      // std::pairのため
#include "CollisionShapeType.h"
#include <DxLib.h>      // VECTORのため

// 前方宣言
class ICollisionComponent;

// 衝突情報を保持するための構造体
struct CollisionInfo
{
    VECTOR normal; // 押し出す方向（正規化済みベクトル）
    float depth;   // めり込みの深さ
};

// 衝突判定関数の型定義を、CollisionInfoを返すように修正
using CollisionFunc = std::function<bool(const ICollisionComponent&, const ICollisionComponent&, CollisionInfo&)>;

// 衝突形状のペアを定義する構造体 (マップのキーとして使用)
// 順序を問わないように、必ずShapeTypeの値が小さい方をfirstに格納する
struct CollisionPair
{
    CollisionShapeType first;
    CollisionShapeType second;

    CollisionPair(CollisionShapeType t1, CollisionShapeType t2) {
        if (t1 < t2) {
            first = t1;
            second = t2;
        }
        else {
            first = t2;
            second = t1;
        }
    }

    // std::map のキーとして使えるように、比較演算子をオーバーロード
    bool operator<(const CollisionPair& other) const {
        if (first < other.first) return true;
        if (first > other.first) return false;
        return second < other.second;
    }
};