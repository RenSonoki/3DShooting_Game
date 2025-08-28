#pragma once
#include "IScene.h"
#include <memory>

// 前方宣言
class EntitySystem; // ★ EntitySystemを追加
class UISystem;
class CameraSystem;
class Game;

/**
 * @class TitleScene
 * @brief ゲームのタイトルシーン。UIとカメラの管理を行う。
 */
class TitleScene : public IScene
{
public:
    TitleScene();
    ~TitleScene() override;

    // ISceneインターフェースの実装
    void OnEnter(Game& game) override;
    void OnExit(Game& game) override;
    void Update(float deltaTime, Game& game) override;
    void Draw(const Game& game) const override;

private:
    // このシーンが所有するシステム群
    std::unique_ptr<EntitySystem> m_entitySystem; // ★ EntitySystemを追加
    std::unique_ptr<UISystem> m_uiSystem;
    std::unique_ptr<CameraSystem> m_cameraSystem;

    // シーン固有の状態
    bool m_isStartRequested;

    // リソースハンドル
    int m_bgmHandle;
    int m_decideSEHandle;
};