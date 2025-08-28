#pragma once
#include <memory>

// 前方宣言
class IScene;

// ゲーム全体の制御を行うクラス
class Game
{
private:
    // NOTE: 所有権を持つポインタで現在のシーンを管理
	// unique_ptrとshared_ptrの違いに注意
	// unique_ptrは所有権を一つのオブジェクトに限定し、メモリ管理を自動化する
	// 共有ポインタであるshared_ptrは複数のオブジェクトで所有権を共有できるが、
	// その分オーバーヘッドが大きくなるため、ここではunique_ptrを使用
    std::unique_ptr<IScene> m_currentScene;

public:
    Game();
    ~Game();

    void Update(float deltaTime);
    void Draw() const;

    void ChangeScene(std::unique_ptr<IScene> newScene);
};