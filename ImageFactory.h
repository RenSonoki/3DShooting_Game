#pragma once

#include <unordered_map>
#include <string>
#include <DxLib.h>

// 画像リソースの読み込みとキャッシュ管理を行うファクトリー
class ImageFactory {
public:
    // 画像を読み込み（キャッシュされていれば再利用）
    int Load(const std::wstring& path);

    // 全画像の解放
    void Clear();

private:
    std::unordered_map<std::wstring, int> m_cache; // パスとハンドルの対応表
};