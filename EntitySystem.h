#pragma once
#include <vector>
#include <memory>
#include <string>

class Entity;

// �V�[���ɑ��݂���S�Ắu�Q�[�����[���h�́v�G���e�B�e�B�̃��C�t�T�C�N�����Ǘ�����N���X
class EntitySystem
{
private:
    void ApplyPendingActions();

    std::vector<std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Entity>> m_pendingAdd;
    std::vector<Entity*> m_pendingRemoval;

    bool m_isUpdating = false;

public:
    EntitySystem() = default;
    ~EntitySystem() = default;

	// -- �V���O���g�����̂��߂̍폜 --

    EntitySystem(const EntitySystem&) = delete;
    EntitySystem& operator=(const EntitySystem&) = delete;

    void AddEntity(std::shared_ptr<Entity> entity);
    void RemoveEntity(std::shared_ptr<Entity> entity);
    const std::vector<std::shared_ptr<Entity>>& GetEntities() const;

    size_t GetEntityCount() const;

    void StartAll();
    void UpdateAll(float deltaTime);
    void DrawAll() const;
    void Clear();


};