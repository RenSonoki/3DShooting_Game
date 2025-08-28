#include "ModelAnimatorComponent.h"
#include "Entity.h"
#include "RenderModelComponent.h"
#include <DxLib.h>
#include <cassert>

ModelAnimatorComponent::ModelAnimatorComponent() = default;
ModelAnimatorComponent::~ModelAnimatorComponent()
{
    for (auto const& [name, handle] : m_attachedAnimHandles) {
        if (handle != -1) MV1DeleteModel(handle);
    }
}

void ModelAnimatorComponent::Start()
{
    m_renderComponent = GetOwner()->GetComponent<RenderModelComponent>();
    assert(!m_renderComponent.expired() && "ModelAnimatorComponent requires a RenderModelComponent.");
    BuildAnimationCache();
}

void ModelAnimatorComponent::Update(float deltaTime)
{
    auto renderComp = m_renderComponent.lock();
    if (!renderComp || m_playHandle == -1) return;

    const int modelHandle = renderComp->GetModelHandle();
    if (modelHandle == -1) return;

    // 1. 再生時間を進める
    m_currentAnimTime += deltaTime * 30.0f * m_currentAnimSpeed; // DxLibの時間は30FPS基準が多い

    // 2. ループ再生の場合、総時間を超えたら時間を巻き戻す
    if (m_isLooping && m_currentAnimTotalTime > 0.0f)
    {
        m_currentAnimTime = fmod(m_currentAnimTime, m_currentAnimTotalTime);
    }

    // 3. モデルに現在の再生時間を設定
    MV1SetAttachAnimTime(modelHandle, m_playHandle, m_currentAnimTime);

    printfDx(L"Animating: %ls, Current Time: %.2f / Total Time: %.2f \n", m_currentAnimName.c_str(), m_currentAnimTime, m_currentAnimTotalTime);
}

ComponentID ModelAnimatorComponent::GetID() const { return ComponentID::Animator; }

bool ModelAnimatorComponent::AttachAnimation(const std::wstring& animName, const std::wstring& filePath)
{
    if (FindAnimationIndexByName(animName) != -1 || m_attachedAnimHandles.count(animName) > 0) return false;
    int animHandle = MV1LoadModel(filePath.c_str());
    if (animHandle != -1) {
        m_attachedAnimHandles[animName] = animHandle;
        return true;
    }
    return false;
}

void ModelAnimatorComponent::PlayAnimationByName(const std::wstring& name, bool loop, float speed)
{
    auto renderComp = m_renderComponent.lock();
    if (!renderComp) return;
    const int modelHandle = renderComp->GetModelHandle();
    if (modelHandle == -1) return;

    if (m_currentAnimName == name) return;

    int animIndex = -1;
    int animSrcHandle = modelHandle;

    auto it = m_attachedAnimHandles.find(name);
    if (it != m_attachedAnimHandles.end()) {
        animIndex = 0;
        animSrcHandle = it->second;
    }
    else {
        animIndex = FindAnimationIndexByName(name);
    }

    if (animIndex == -1) return;

    if (m_playHandle != -1) {
        MV1DetachAnim(modelHandle, m_playHandle);
    }

    m_playHandle = MV1AttachAnim(modelHandle, animIndex, animSrcHandle, loop ? TRUE : FALSE);

    // ★ 修正点: 再生に必要な情報をメンバー変数に保存
    if (m_playHandle != -1)
    {
        m_currentAnimName = name;
        m_currentAnimTime = 0.0f;
        m_isLooping = loop;
        m_currentAnimSpeed = speed;
        // 総再生時間を取得して保存
        m_currentAnimTotalTime = MV1GetAttachAnimTotalTime(modelHandle, m_playHandle);
        // 再生時間を0にリセット
        MV1SetAttachAnimTime(modelHandle, m_playHandle, 0.0f);
    }

    printfDx(L"Play Start: %ls, Total Time is: %.2f \n", m_currentAnimName.c_str(), m_currentAnimTotalTime);
}
void ModelAnimatorComponent::StopAnimation()
{
    auto renderComp = m_renderComponent.lock();
    if (!renderComp || m_playHandle == -1) return;
    const int modelHandle = renderComp->GetModelHandle();
    if (modelHandle != -1) MV1DetachAnim(modelHandle, m_playHandle);
    m_playHandle = -1;
    m_currentAnimName = L"";
}

bool ModelAnimatorComponent::IsPlaying() const { return m_playHandle != -1; }
std::wstring ModelAnimatorComponent::GetCurrentAnimationName() const { return m_currentAnimName; }

void ModelAnimatorComponent::BuildAnimationCache()
{
    auto renderComp = m_renderComponent.lock();
    if (!renderComp) return;
    const int modelHandle = renderComp->GetModelHandle();
    if (modelHandle == -1) return;

    m_animNameToIndex.clear();
    m_totalAnimNum = MV1GetAnimNum(modelHandle);
    for (int i = 0; i < m_totalAnimNum; ++i) {
        const wchar_t* animName = MV1GetAnimName(modelHandle, i);
        if (animName) m_animNameToIndex[animName] = i;
    }
}

int ModelAnimatorComponent::FindAnimationIndexByName(const std::wstring& name) const
{
    auto it = m_animNameToIndex.find(name);
    return (it != m_animNameToIndex.end()) ? it->second : -1;
}