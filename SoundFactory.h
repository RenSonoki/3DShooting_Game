#pragma once

#include <unordered_map>
#include <string>
#include <DxLib.h>

// サウンドリソース（WAV/MP3/BGM/SE）を管理するファクトリークラス
class SoundFactory {
public:
    // サウンドの読み込み（キャッシュされていれば再利用）
    int Load(const std::wstring& path);

    // 全サウンドの解放（DeleteSoundMem）
    void Clear();

private:
    std::unordered_map<std::wstring, int> m_cache; // パスとハンドルのマップ
};