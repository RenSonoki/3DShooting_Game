#include "CameraSystem.h"
#include "Entity.h"
#include "CameraComponent.h"
#include <algorithm>

void CameraSystem::Register(const std::shared_ptr<Entity>& cameraEntity)
{
    if (!cameraEntity) return;

    auto camera = cameraEntity->GetComponent<CameraComponent>();
    if (camera)
    {
        // 重複がないか確認
        auto it = std::find(m_cameras.begin(), m_cameras.end(), camera);
        if (it == m_cameras.end())
        {
            m_cameras.push_back(camera);
            // 最初に追加されたカメラをアクティブにする
            if (m_activeCamera.expired())
            {
                m_activeCamera = camera;
            }
        }
    }
}

void CameraSystem::Unregister(const std::shared_ptr<Entity>& cameraEntity)
{
    if (!cameraEntity) return;
    auto camera = cameraEntity->GetComponent<CameraComponent>();
    if (camera)
    {
        // 【修正点】即時削除ではなく、削除待ちリストに追加する
        m_pendingRemoval.push_back(camera);
    }
}

void CameraSystem::SetActiveCamera(const std::shared_ptr<CameraComponent>& camera)
{
    // 指定されたカメラが登録済みか確認（nullptrも考慮）
    if (camera) {
        auto it = std::find(m_cameras.begin(), m_cameras.end(), camera);
        if (it != m_cameras.end())
        {
            m_activeCamera = camera;
        }
    }
    else {
        m_activeCamera.reset();
    }
}

std::shared_ptr<CameraComponent> CameraSystem::GetActiveCamera() const
{
    return m_activeCamera.lock();
}

void CameraSystem::ApplyActiveCamera()
{
    // 1. まず、安全に削除処理を行う
    ApplyPendingRemovals();

    // 2. アクティブなカメラを取得して設定を適用
    if (auto active = m_activeCamera.lock())
    {
        // 【修正点】メソッド名をActivate()に統一
        active->Activate();
    }
}

void CameraSystem::Clear()
{
    m_cameras.clear();
    m_pendingRemoval.clear();
    m_activeCamera.reset();
}

// private:
void CameraSystem::ApplyPendingRemovals()
{
    if (m_pendingRemoval.empty()) return;

    // 削除待ちリストにあるカメラを、メインのリストから除去する
    m_cameras.erase(
        std::remove_if(m_cameras.begin(), m_cameras.end(),
            [this](const auto& cam) {
                return std::find(m_pendingRemoval.begin(), m_pendingRemoval.end(), cam) != m_pendingRemoval.end();
            }),
        m_cameras.end());

    m_pendingRemoval.clear();
}