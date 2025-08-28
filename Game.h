#pragma once
#include <memory>

// �O���錾
class IScene;

// �Q�[���S�̂̐�����s���N���X
class Game
{
private:
    // NOTE: ���L�������|�C���^�Ō��݂̃V�[�����Ǘ�
	// unique_ptr��shared_ptr�̈Ⴂ�ɒ���
	// unique_ptr�͏��L������̃I�u�W�F�N�g�Ɍ��肵�A�������Ǘ�������������
	// ���L�|�C���^�ł���shared_ptr�͕����̃I�u�W�F�N�g�ŏ��L�������L�ł��邪�A
	// ���̕��I�[�o�[�w�b�h���傫���Ȃ邽�߁A�����ł�unique_ptr���g�p
    std::unique_ptr<IScene> m_currentScene;

public:
    Game();
    ~Game();

    void Update(float deltaTime);
    void Draw() const;

    void ChangeScene(std::unique_ptr<IScene> newScene);
};