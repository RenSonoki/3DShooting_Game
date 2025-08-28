#pragma once
#include "Entity.h"

// 弾エンティティの設計図
// コンストラクタからロジックを分離し、シンプルなクラスにする
class BulletEntity : public Entity
{
public:
    BulletEntity();
};