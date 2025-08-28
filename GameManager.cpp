#include "GameManager.h"

GameManager& GameManager::GetInstance()
{
    // NOTE: この関数が初めて呼ばれた時に一度だけインスタンスが生成される
    static GameManager instance;
    return instance;
}

void GameManager::AddScore(int score)
{
    m_score += score;
}

void GameManager::Reset()
{
    m_score = 0;
}

int GameManager::GetScore() const
{
    return m_score;
}