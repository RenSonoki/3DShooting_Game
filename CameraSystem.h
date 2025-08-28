#pragma once
#include <memory>
#include <vector>
#include <string>

// 前方宣言
class CameraComponent;
class Entity;

// 複数のカメラを管理し、アクティブなカメラをDxLibに適用するシステム
class CameraSystem
{
public:
    CameraSystem() = default;
    ~CameraSystem() = default;

    // このシステムはコピーできない
    CameraSystem(const CameraSystem&) = delete;
    CameraSystem& operator=(const CameraSystem&) = delete;

    void Register(const std::shared_ptr<Entity>& cameraEntity);
    void Unregister(const std::shared_ptr<Entity>& cameraEntity);

    void SetActiveCamera(const std::shared_ptr<CameraComponent>& camera);
    std::shared_ptr<CameraComponent> GetActiveCamera() const;

    // 毎フレーム、アクティブカメラをDxLibに反映（更新処理も兼ねる）
    void ApplyActiveCamera();

    void Clear();

private:
    // 遅延削除を実行するヘルパー関数
    void ApplyPendingRemovals();

    // CameraComponentへのポインタを直接管理する
    std::vector<std::shared_ptr<CameraComponent>> m_cameras;
    std::weak_ptr<CameraComponent> m_activeCamera;

    // 遅延削除用のリストを追加
    std::vector<std::shared_ptr<CameraComponent>> m_pendingRemoval;
};