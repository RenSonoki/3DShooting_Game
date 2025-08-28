#include "BulletPrototype.h"
#include "BulletBuilder.h" // Builderを使って構築する

// ★ デフォルトコンストラクタの実装
BulletPrototype::BulletPrototype()
    : m_modelPath(L"")
    , m_speed(100.0f)
    , m_lifetime(3.0f)
{
}

// ★ セッターの実装
void BulletPrototype::SetModel(const std::wstring& modelPath) { m_modelPath = modelPath; }
void BulletPrototype::SetSpeed(float speed) { m_speed = speed; }
void BulletPrototype::SetLifetime(float lifetime) { m_lifetime = lifetime; }

// CloneメソッドはBuilderを使う
std::shared_ptr<BulletEntity> BulletPrototype::Clone(const VECTOR& position, const VECTOR& direction) const
{
    BulletBuilder builder;
    auto bullet = builder.SetPosition(position)
        .SetDirection(direction)
        .SetModel(m_modelPath)
        .SetSpeed(m_speed)
        .SetLifetime(m_lifetime)
        .Build();

    return bullet;
}