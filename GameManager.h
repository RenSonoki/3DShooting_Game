#pragma once

// ゲームの状態やルールを管理するシングルトン
class GameManager
{
private:
    // NOTE: privateコンストラクタで外部からの生成を禁止
    GameManager() = default;

    int m_score = 0;

    // 今後、残りライフ数などもここに追加していく
    // int m_lives = 3;

public:
    // 唯一のインスタンスを取得
    static GameManager& GetInstance();

    // シングルトンなのでコピーとムーブは禁止
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    // --- ゲームロジック ---
    void AddScore(int score);
    void Reset();
    int GetScore() const;
};