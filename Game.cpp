#include "Game.h"
#include "IScene.h"
#include "TitleScene.h"


Game::Game()
{
    // �N�����Ƀ^�C�g���V�[���ɑJ��
    ChangeScene(std::make_unique<TitleScene>());
}

// NOTE: ���̎�����cpp���ɂ��邱�ƂŁA�R���p�C����IScene�̃f�X�g���N�^��m�邱�Ƃ��ł���
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

	// NOTE: std::move���g�����ƂŁAnewScene�̏��L����m_currentScene�Ɉڂ�
	// ����ɂ��AnewScene��nullptr�ɂȂ�A���������[�N��h��
	// move��unique_ptr�̏��L�����ړ����邽�߂Ɏg�p����܂��B(C+11�ȍ~�̋@�\)
    m_currentScene = std::move(newScene);

    if (m_currentScene)
    {
        m_currentScene->OnEnter(*this);
    }
}