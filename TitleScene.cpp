#include "TitleScene.h"
#include "Game.h"
#include "PlayScene.h"

// 必要なモジュールをインクルード
#include "VirtualScreenManager.h"
#include "CameraBuilder.h"
#include "CameraEntity.h"
#include "EntitySystem.h" // ★ EntitySystemを追加
#include "UISystem.h"
#include "CameraSystem.h"
#include "UIImage.h"
#include "UIButton.h"
#include "LogoWaveAnimator.h"
#include "ResourceManager.h"
#include "ResourceTraits.h"
#include <DxLib.h>

// TitleSceneのコンストラクタ
TitleScene::TitleScene()
    : m_entitySystem(std::make_unique<EntitySystem>()) // ★ EntitySystemを生成
    , m_uiSystem(std::make_unique<UISystem>())
    , m_cameraSystem(std::make_unique<CameraSystem>())
    , m_isStartRequested(false)
    , m_bgmHandle(-1)
    , m_decideSEHandle(-1)
{
}

TitleScene::~TitleScene() {}

// シーン開始時の初期化処理
void TitleScene::OnEnter(Game& game)
{
    // --- 1. リソースの読み込み ---
    m_bgmHandle = ResourceManager::Instance().Get<SoundTag>(L"Assets/Sounds/title_bgm.mp3");
    m_decideSEHandle = ResourceManager::Instance().Get<SoundTag>(L"Assets/Sounds/decide.mp3");
    if (m_bgmHandle != -1) PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);

    // --- 2. カメラの生成 ---
    // Builderで生成したカメラをshared_ptrに変換
    auto cameraEntity = CameraBuilder().Build();

    // ★ 各システムにカメラを登録
    m_cameraSystem->Register(cameraEntity);
    m_entitySystem->AddEntity(cameraEntity); // カメラもエンティティとして管理

    // --- 3. UI要素の配置 ---
    const auto& vsm = VirtualScreenManager::GetInstance();
    const float virtualWidth = static_cast<float>(vsm.GetVirtualWidth());

    // ロゴの生成
    auto logo = std::make_unique<UIImage>(L"Assets/UI/titleLogo.png");
    auto& logoTransform = logo->GetTransform();
    logoTransform.SetPivot({ 0.5f, 0.5f });
    logoTransform.SetPosition({ virtualWidth / 2.5f, 200.0f });
    logoTransform.SetScale({ 0.7f, 0.7f }); // ★ スケールを縦横50%に設定

    auto animator = std::make_unique<LogoWaveAnimator>();
    animator->Setup(200.0f, 15.0f, 2.0f);
    logo->SetAnimator(std::move(animator));
    m_uiSystem->AddElement(std::move(logo));

    // スタートボタンの生成
    auto button = std::make_unique<UIButton>(
        L"Assets/UI/startButton_normal.png",
        L"Assets/UI/startButton_hover.png"
    );
    auto& buttonTransform = button->GetTransform();
    buttonTransform.SetPivot({ 0.5f, 0.5f });
    buttonTransform.SetPosition({ virtualWidth / 2.5f, 400.0f });
    buttonTransform.SetScale({ 0.5f, 0.5f }); // ★ スケールを縦横50%に設定

    button->SetOnClick([this]() {
        if (m_decideSEHandle != -1) PlaySoundMem(m_decideSEHandle, DX_PLAYTYPE_BACK);
        m_isStartRequested = true;
        });
    m_uiSystem->AddElement(std::move(button));
}

// シーン終了時の処理
void TitleScene::OnExit(Game& game)
{
    if (m_bgmHandle != -1) StopSoundMem(m_bgmHandle);
    m_entitySystem->Clear();
    m_uiSystem->Clear();
    m_cameraSystem->Clear();
}

// フレームごとの更新処理
void TitleScene::Update(float deltaTime, Game& game)
{
    m_entitySystem->UpdateAll(deltaTime);
    m_uiSystem->Update(deltaTime);

    if (m_isStartRequested)
    {
        game.ChangeScene(std::make_unique<PlayScene>());
    }
}

// フレームごとの描画処理
void TitleScene::Draw(const Game& game) const
{
    // --- 3D描画パス ---
    // (この部分は変更なし)
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    m_cameraSystem->ApplyActiveCamera();
    // m_entitySystem->DrawAll(); 


    // --- 2D描画パス ---
    // 💡 古いDXLibでも動作する、より確実な2D描画設定
    const auto& vsm = VirtualScreenManager::GetInstance();

    // 1. 深度バッファを無効化 (これは必須)
    SetUseZBuffer3D(FALSE);
    SetWriteZBuffer3D(FALSE);

    // 2. カメラを正射影（2D用）に設定
    SetupCamera_Ortho(vsm.GetVirtualHeight());

    // 3. ★★★画面の中心点を明示的にリセット★★★
    // 3Dカメラによって画面の中心点がずらされている可能性を考慮し、
    // 2D描画の原点(0,0)が左上隅に来るように強制的にリセットします。
    SetCameraScreenCenter(0.0f, 0.0f);


    // --- これで描画準備は完璧です ---

    // 背景を描画
    int bgHandle = ResourceManager::Instance().Get<ImageTag>(L"Assets/UI/background_title.png");
    if (bgHandle != -1)
    {
        DrawExtendGraph(0, 0, vsm.GetVirtualWidth(), vsm.GetVirtualHeight(), bgHandle, TRUE);
    }

    // UIを描画
    m_uiSystem->Draw();
}