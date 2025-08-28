#include "Game.h"
#include "IScene.h"
#include "TitleScene.h"


Game::Game()
{
    // 起動時にタイトルシーンに遷移
    ChangeScene(std::make_unique<TitleScene>());
}

// NOTE: この実装がcpp側にあることで、コンパイラはISceneのデストラクタを知ることができる
Game::~Game() = default;

void Game::Update(float deltaTime)
{
    if (m_currentScene)
    {
        m_currentScene->Update(deltaTime, *this);
    }
}

void Game::Draw() const
{
    if (m_currentScene)
    {
        m_currentScene->Draw(*this);
    }
}

void Game::ChangeScene(std::unique_ptr<IScene> newScene)
{
    if (m_currentScene)
    {
        m_currentScene->OnExit(*this);
    }

	// NOTE: std::moveを使うことで、newSceneの所有権をm_currentSceneに移す
	// これにより、newSceneはnullptrになり、メモリリークを防ぐ
	// moveはunique_ptrの所有権を移動するために使用されます。(C+11以降の機能)
    m_currentScene = std::move(newScene);

    if (m_currentScene)
    {
        m_currentScene->OnEnter(*this);
    }
}