#define NOMINMAX
#include <windows.h>
#include <DxLib.h>

#include "Game.h"
#include "VirtualScreenManager.h"
#include "DebugRenderer.h"
#include "Random.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // NOTE: デバッグのためログ出力
    SetOutApplicationLogValidFlag(TRUE);

    // 仮想画面サイズ
    const int VIRTUAL_WIDTH = 1280;
    const int VIRTUAL_HEIGHT = 720;

    ChangeWindowMode(TRUE);

    // DxLibの初期化と画面設定
    SetGraphMode(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, 32);
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // 3D描画のための設定
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    SetCameraNearFar(1.0f, 10000.0f);

	SetBackgroundColor(0, 255, 0);

    // 描画先をバックバッファに設定
    SetDrawScreen(DX_SCREEN_BACK);

    // --- 各シングルトンマネージャの初期化 ---

    // 仮想画面初期化 (GetInstance() を経由して呼び出す)
    VirtualScreenManager::GetInstance().Init(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    // 乱数エンジンの初期化
    // NOTE: GetInstance()が初めて呼ばれた時にコンストラクタが走り、シードが設定される
    Random::GetInstance();

    // Gameインスタンス生成
    Game game;

    LONGLONG prevTime = GetNowHiPerformanceCount();

    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // DeltaTimeの計算
        LONGLONG currentTime = GetNowHiPerformanceCount();
        float deltaTime = (currentTime - prevTime) / 1000000.0f;
        prevTime = currentTime;

        // --- 1. 更新 ---
        game.Update(deltaTime);

        // --- 2. 描画 ---
        ClearDrawScreen();

        // 仮想画面への描画開始
        VirtualScreenManager::GetInstance().BeginDraw();

        // シーンの内容を描画
        game.Draw();

#if _DEBUG // デバッグビルドの時だけ実行
        DebugRenderer::GetInstance().RenderAll();
#endif

        // 仮想画面の内容を実画面へ転送
        VirtualScreenManager::GetInstance().EndDraw();

        // 実画面をディスプレイに反映
        ScreenFlip();
    }

    DxLib_End();
    return 0;
}