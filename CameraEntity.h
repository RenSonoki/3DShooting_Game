#pragma once
#include <memory>

#include "Entity.h"

class TransformComponent;
class CameraComponent;
class ThirdPersonCameraComponent;


class CameraEntity : public Entity
{
public:
    CameraEntity();

    std::shared_ptr<CameraComponent> GetCameraComponent() const;
    std::shared_ptr<TransformComponent> GetTransformComponent() const;
    std::shared_ptr<ThirdPersonCameraComponent> GetThirdPersonCamera() const;
};