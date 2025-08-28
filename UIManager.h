#pragma once
#include "UISystem.h" // UISystemの前方宣言ではなく、完全な定義が必要
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>

// 複数のUIシステム（レイヤー）を名前で管理する統一マネージャ
class UIManager
{
public:
    // UIレイヤーを追加・登録（所有権がUIManagerに移動する）
    void AddSystem(const std::string& name, std::unique_ptr<UISystem> system);

    // 名前でレイヤーを取得（所有権は渡さないため、生ポインタを返す）
    UISystem* GetSystem(const std::string& name) const;

    // 指定レイヤーを削除
    void RemoveSystem(const std::string& name);

    // 全UIレイヤーを描画（レイヤー深度を考慮）
    void DrawAll(); // 引数を削除

    // 全UIレイヤーを更新
    void UpdateAll(float deltaTime);

    // 全レイヤーを削除
    void Clear();

private:
    // 【修正点】std::unique_ptrで所有権を明確化
    std::unordered_map<std::string, std::unique_ptr<UISystem>> m_systems;
};