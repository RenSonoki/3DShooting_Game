#pragma once
#include "IEntityPrototype.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

// Prototype パターンを用いたエンティティ生成管理クラス（Singleton）
class PrototypeManager
{
public:
    static PrototypeManager& Instance();

    // 所有権を移動させるため、unique_ptrでプロトタイプを受け取る
    void Register(const std::wstring& id, std::unique_ptr<IEntityPrototype> prototype);

    // 複製（通常）
    std::shared_ptr<Entity> Create(const std::wstring& id) const;

    // 位置付き複製
    std::shared_ptr<Entity> CreateAt(const std::wstring& id, const VECTOR& pos) const;

    // 存在確認
    bool Has(const std::wstring& id) const;

    // 登録済み ID 一覧取得
    std::vector<std::wstring> GetAllIDs() const;

    // 全削除
    void Clear();

private:
    PrototypeManager() = default;
    PrototypeManager(const PrototypeManager&) = delete;
    PrototypeManager& operator=(const PrototypeManager&) = delete;

    // 【修正】unique_ptrでプロトタイプを排他的に所有
    std::unordered_map<std::wstring, std::unique_ptr<IEntityPrototype>> m_prototypes;
};