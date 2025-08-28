#pragma once

class Game;

// シーンの抽象基底クラス
class IScene
{
public:
    virtual ~IScene() = default;

    virtual void OnEnter(Game& game) = 0;
    virtual void Update(float deltaTime, Game& game) = 0;
    virtual void OnExit(Game& game) = 0;

    virtual void Draw(const Game& game) const = 0;
};