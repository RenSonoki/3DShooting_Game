#pragma once

#include <unordered_map>
#include <string>
#include <DxLib.h>

// モデルハンドルとアニメーション名を元にインデックスを解決・キャッシュするファクトリー
class AnimationFactory
{
public:
    // アニメーション名に対応するインデックスを取得（未取得ならMV1GetAnimIndexで取得してキャッシュ）
    int GetIndex(int modelHandle, const std::wstring& animName);

    // キャッシュの全削除
    void Clear();

private:
    // キー：モデルハンドルとアニメーション名の組み合わせ
    using Key = std::pair<int, std::wstring>;

    struct KeyHash {
        std::size_t operator()(const Key& key) const {
            return std::hash<int>()(key.first) ^ std::hash<std::wstring>()(key.second);
        }
    };

    std::unordered_map<Key, int, KeyHash> m_cache;
};