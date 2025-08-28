#include "CameraEntity.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "ThirdPersonCameraComponent.h"

CameraEntity::CameraEntity()
{
    SetTag(L"Camera");
}

// ƒQƒbƒ^[‚ÌÀ‘•
std::shared_ptr<CameraComponent> CameraEntity::GetCameraComponent() const { return GetComponent<CameraComponent>(); }
std::shared_ptr<TransformComponent> CameraEntity::GetTransformComponent() const { return GetComponent<TransformComponent>(); }
std::shared_ptr<ThirdPersonCameraComponent> CameraEntity::GetThirdPersonCamera() const{ return GetComponent<ThirdPersonCameraComponent>(); }