#pragma once
#include "IScene.h"
#include <memory>
#include <Windows.h>

// --- 前方宣言 ---
// このシーンが利用する主要なクラスを前方宣言しておくことで、
// ヘッダーファイル間の不要なインクルードを減らし、コンパイル時間を短縮します。
class EntitySystem;
class MovementSystem;
class CameraSystem;
class UIManager;
class PlayerEntity;
class EnemySpawner;
class Game;
class UIText;

/**
 * @class PlayScene
 * @brief ゲームプレイ中の状態を管理するシーンクラス。
 * @details このクラスは、各種システムやロジック専門クラスを所有し、
 * それらに処理を委任する「現場監督」のような役割を担います。
 */
class PlayScene : public IScene
{
public:
    PlayScene();
    ~PlayScene() override;

    // ISceneインターフェースの実装
    void OnEnter(Game& game) override;
    void OnExit(Game& game) override;
    void Update(float deltaTime, Game& game) override;
    void Draw(const Game& game) const override;

private:
    // UIの表示内容を更新するヘルパー関数
    void UpdateUI();

    // --- このシーンが所有するシステム群 ---
    std::unique_ptr<EntitySystem> m_entitySystem;     // ゲーム内オブジェクト(Entity)のライフサイクルを管理
    std::unique_ptr<MovementSystem> m_movementSystem;   // 物理演算（移動処理）を専門に担当
    std::unique_ptr<CameraSystem> m_cameraSystem;     // カメラの管理と設定適用を担当
    std::unique_ptr<UIManager> m_uiManager;          // UIレイヤー(UISystem)の管理を担当

    // --- ロジック専門クラス ---
    std::unique_ptr<EnemySpawner> m_enemySpawner;     // 敵の出現ロジックを専門に担当

    // --- 主要なオブジェクトへの参照 ---
    std::shared_ptr<PlayerEntity> m_player; // プレイヤーへの参照

    // UI要素へのポインタ（UIの表示更新に使う）
    UIText* m_scoreText = nullptr;
    UIText* m_hpText = nullptr;

    // --- リソースハンドル ---
    int m_bgmHandle = -1;
};