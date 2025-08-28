#define NOMINMAX
#include <windows.h>
#include <DxLib.h>

#include "Game.h"
#include "VirtualScreenManager.h"
#include "DebugRenderer.h"
#include "Random.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // NOTE: �f�o�b�O�̂��߃��O�o��
    SetOutApplicationLogValidFlag(TRUE);

    // ���z��ʃT�C�Y
    const int VIRTUAL_WIDTH = 1280;
    const int VIRTUAL_HEIGHT = 720;

    ChangeWindowMode(TRUE);

    // DxLib�̏������Ɖ�ʐݒ�
    SetGraphMode(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, 32);
    if (DxLib_Init() == -1)
    {
        return -1;
    }

    // 3D�`��̂��߂̐ݒ�
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    SetCameraNearFar(1.0f, 10000.0f);

	SetBackgroundColor(0, 255, 0);

    // �`�����o�b�N�o�b�t�@�ɐݒ�
    SetDrawScreen(DX_SCREEN_BACK);

    // --- �e�V���O���g���}�l�[�W���̏����� ---

    // ���z��ʏ����� (GetInstance() ���o�R���ČĂяo��)
    VirtualScreenManager::GetInstance().Init(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    // �����G���W���̏�����
    // NOTE: GetInstance()�����߂ČĂ΂ꂽ���ɃR���X�g���N�^������A�V�[�h���ݒ肳���
    Random::GetInstance();

    // Game�C���X�^���X����
    Game game;

    LONGLONG prevTime = GetNowHiPerformanceCount();

    // ���C�����[�v
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // DeltaTime�̌v�Z
        LONGLONG currentTime = GetNowHiPerformanceCount();
        float deltaTime = (currentTime - prevTime) / 1000000.0f;
        prevTime = currentTime;

        // --- 1. �X�V ---
        game.Update(deltaTime);

        // --- 2. �`�� ---
        ClearDrawScreen();

        // ���z��ʂւ̕`��J�n
        VirtualScreenManager::GetInstance().BeginDraw();

        // �V�[���̓��e��`��
        game.Draw();

#if _DEBUG // �f�o�b�O�r���h�̎��������s
        DebugRenderer::GetInstance().RenderAll();
#endif

        // ���z��ʂ̓��e������ʂ֓]��
        VirtualScreenManager::GetInstance().EndDraw();

        // ����ʂ��f�B�X�v���C�ɔ��f
        ScreenFlip();
    }

    DxLib_End();
    return 0;
}