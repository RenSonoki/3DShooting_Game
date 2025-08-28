#include "PlayScene.h"
#include "Game.h"
#include "TitleScene.h"

// 必要なモジュールを全てインクルード
#include "VirtualScreenManager.h"
#include "ResourceManager.h"
#include "ResourceTraits.h"
#include "ColliderManager.h"
#include "GameManager.h"

// システム
#include "EntitySystem.h"
#include "MovementSystem.h"
#include "CameraSystem.h"
#include "UIManager.h"
#include "UISystem.h"

// 設計図（BuilderとPrototype）
#include "PlayerBuilder.h"
#include "CameraBuilder.h"
#include "EnemyPrototype.h"
#include "BulletPrototype.h"

// その他
#include "EnemySpawner.h"
#include "PlayerEntity.h"
#include "PlayerControllerComponent.h"
#include "ThirdPersonCameraComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "CameraEntity.h"
#include "UIText.h"
#include <DxLib.h>
#include <string>

// コンストラクタ: このシーンで利用するシステム群のインスタンスを生成
PlayScene::PlayScene()
    : m_entitySystem(std::make_unique<EntitySystem>())
    , m_movementSystem(std::make_unique<MovementSystem>())
    , m_cameraSystem(std::make_unique<CameraSystem>())
    , m_uiManager(std::make_unique<UIManager>())
    , m_player(nullptr)
    , m_enemySpawner(nullptr)
    , m_bgmHandle(-1)
{
}

PlayScene::~PlayScene() {}

// シーン開始時の初期化処理
void PlayScene::OnEnter(Game& game)
{
    // --- 1. グローバルなマネージャーとシステムの初期化 ---
    GameManager::GetInstance().Reset();
    ColliderManager::GetInstance().Init(1000.0f, 1000.0f, 50.0f); // ワールドサイズとグリッドサイズを指定

    // 💡 ★★★ この部分を追加 ★★★
    // シーンのデフォルトライトを作成します。
    // これにより、3Dモデルが照らされて見えるようになります。
    SetLightEnable(TRUE);
	ChangeLightTypePoint(VECTOR{ 0.0f, 100.0f, 0.0f }, GetColor(255, 255, 255), 1.0f, 1.0f, 1.0f); // 点光源
    SetLightDirection(VGet(0.5f, -2.0f, 0.5f));  // 斜め上からの光

    // --- 2. リソース読み込み ---
    m_bgmHandle = ResourceManager::Instance().Get<SoundTag>(L"Assets/Sounds/play_bgm.mp3");
    if (m_bgmHandle != -1) PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);

    // --- 3. 設計図（プロトタイプ）の準備 ---
    auto bulletProto = std::make_shared<BulletPrototype>();
    bulletProto->SetModel(L"Assets/Models/bullet.mv1");
    bulletProto->SetSpeed(80.0f);

    auto enemyProto = std::make_shared<EnemyPrototype>();
    enemyProto->SetModelPath(L"Assets/Models/enemy.pmx");
    enemyProto->SetSpeed(10.0f);
    enemyProto->SetScore(100);
    // 敵が倒された時にGameManagerのスコアを増やすコールバックを設定
    enemyProto->SetOnDestroyCallback([](int score)
        {
        GameManager::GetInstance().AddScore(score);
        });

    // --- 4. プレイヤーの生成 ---
    PlayerBuilder playerBuilder;
    m_player = playerBuilder.SetModel(L"Assets/Models/player.mv1")
        .SetShooterInfo(bulletProto, m_entitySystem.get())
        .Build();
    m_entitySystem->AddEntity(m_player);

    // --- 5. カメラの生成と設定 ---
    auto cameraEntity = CameraBuilder().Build();
    // カメラにプレイヤーを追従させる
    cameraEntity->GetComponent<ThirdPersonCameraComponent>()->SetTarget(m_player->GetTransform());
    // プレイヤーに操作の基準となるカメラを教える
    m_player->GetComponent<PlayerControllerComponent>()->SetCamera(cameraEntity->GetComponent<CameraComponent>());

    m_cameraSystem->Register(cameraEntity);
    m_entitySystem->AddEntity(cameraEntity); // カメラもEntityとして管理

    printfDx(_T("Camera added. Entity Count is now: %d\n"), m_entitySystem->GetEntityCount());

    // --- 6. 敵出現ロジックの生成 ---
    m_enemySpawner = std::make_unique<EnemySpawner>(
        m_entitySystem.get(), enemyProto, m_player->GetTransform()
    );

    // --- 7. UIの生成 ---
    auto gameUI = std::make_unique<UISystem>();

    auto scoreUI = std::make_unique<UIText>(L"Score: 0", 32, GetColor(255, 255, 0));
    scoreUI->GetTransform().SetPosition({ 20, 20 });
    m_scoreText = scoreUI.get(); // 表示更新用にポインタを保持
    gameUI->AddElement(std::move(scoreUI));

    auto hpUI = std::make_unique<UIText>(L"HP: 3", 32, GetColor(255, 255, 255));
    hpUI->GetTransform().SetPosition({ 20, 50 });
    m_hpText = hpUI.get(); // 表示更新用にポインタを保持
    gameUI->AddElement(std::move(hpUI));

    m_uiManager->AddSystem("GameUI", std::move(gameUI));

    // --- 8. 全エンティティのStartを呼び出し、初期化を完了 ---
    m_entitySystem->StartAll();
}

// シーン終了時の後始末
void PlayScene::OnExit(Game& game)
{
    if (m_bgmHandle != -1) StopSoundMem(m_bgmHandle);
    m_entitySystem->Clear();
    m_cameraSystem->Clear();
    m_uiManager->Clear();
    ColliderManager::GetInstance().Clear();
}

// 毎フレームの更新処理
void PlayScene::Update(float deltaTime, Game& game)
{
    // 各専門システムに更新処理を委任
    m_entitySystem->UpdateAll(deltaTime);
    m_movementSystem->Update(m_entitySystem->GetEntities(), deltaTime);
    m_enemySpawner->Update(deltaTime);
    ColliderManager::GetInstance().CheckAllCollisions();
    m_uiManager->UpdateAll(deltaTime);

    // UIの表示内容を更新
    UpdateUI();

    // ゲームオーバー判定
    if (m_player && m_player->IsDead())
    {
        game.ChangeScene(std::make_unique<TitleScene>());
    }
}

// UIの表示内容を更新する
void PlayScene::UpdateUI()
{
    // GameManagerとPlayerから最新の情報を取得してUIに反映
    if (m_scoreText)
        m_scoreText->SetText(L"Score: " + std::to_wstring(GameManager::GetInstance().GetScore()));
    if (m_hpText && m_player)
        m_hpText->SetText(L"HP: " + std::to_wstring(m_player->GetHP()));
}

// 毎フレームの描画処理
void PlayScene::Draw(const Game& game) const
{
    // --- 3D描画パス ---
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);

    // 1. CameraSystemに3Dカメラの設定を適用させる
    m_cameraSystem->ApplyActiveCamera();

    // 2. 地面や3Dモデルを描画する
    //DrawGrid(400.0f, 40, GetColor(80, 80, 80), TRUE);
    m_entitySystem->DrawAll();


    // --- 2D描画パス ---
    const auto& vsm = VirtualScreenManager::GetInstance();

    // 3. 2D描画用に設定を切り替える
    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);
    SetupCamera_Ortho(vsm.GetVirtualHeight());
    SetCameraScreenCenter(0.0f, 0.0f);

    // 4. UIを描画する
    m_uiManager->DrawAll();
}