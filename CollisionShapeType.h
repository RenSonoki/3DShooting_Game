#pragma once

// 衝突形状の識別用列挙型
enum class CollisionShapeType
{
    Sphere,     // 球体
    Box,        // AABBやOBBなどの直方体
    Capsule,    // カプセル型
    Ray,        // レイ（オプション）
    Mesh,       // 多角形 or 三角形メッシュ（将来拡張用）
    Unknown     // 未設定または不明
};