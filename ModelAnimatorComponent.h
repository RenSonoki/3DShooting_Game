#pragma once
#include "ComponentBase.h"
#include <string>
#include <unordered_map>
#include <memory>

class RenderModelComponent;

class ModelAnimatorComponent : public ComponentBase
{
public:
    ModelAnimatorComponent();
    virtual ~ModelAnimatorComponent();

    void Start() override;
    void Update(float deltaTime) override;
    ComponentID GetID() const override;

    bool AttachAnimation(const std::wstring& animName, const std::wstring& filePath);
    void PlayAnimationByName(const std::wstring& name, bool loop = true, float speed = 1.0f);

    void StopAnimation();
    bool IsPlaying() const;
    std::wstring GetCurrentAnimationName() const;

private:
    void BuildAnimationCache();
    int FindAnimationIndexByName(const std::wstring& name) const;

    std::weak_ptr<RenderModelComponent> m_renderComponent;

    // 現在再生中の情報
    int m_playHandle = -1;
    std::wstring m_currentAnimName;

    // ★ 追加: 再生時間と速度を管理するメンバー
    float m_currentAnimTime = 0.0f;
    float m_currentAnimTotalTime = 0.0f;
    float m_currentAnimSpeed = 1.0f;
    bool m_isLooping = false;

    // モデル内蔵アニメーションのキャッシュ
    int m_totalAnimNum = 0;
    std::unordered_map<std::wstring, int> m_animNameToIndex;

    // 外部からアタッチしたアニメーションのハンドル
    std::unordered_map<std::wstring, int> m_attachedAnimHandles;
};