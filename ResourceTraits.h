#pragma once
#include <DxLib.h>
#include <string>

// NOTE: 新しいリソースEffectを追加したい場合
//    1: ResourceTraits<EffectTag>を定義
//    2: ResourceManager.hのAllCachesというtupleにCache<EffectTag>を一行追加
//    3: Get<EffectTag>(...)が自動的に使えるようになります。

// ---リソースの種類を識別するための「タグ」を定義 ---
// NOTE: 中身は空で、型そのものが識別子になります。
struct ModelTag {};
struct ImageTag {};
struct SoundTag {};

// ---リソースごとの特性を定義する「Traits」クラス ---

// デフォルトのテンプレート（中身は未定義）
template<typename T>
struct ResourceTraits;

// ModelTagに対する特殊化
template<>
struct ResourceTraits<ModelTag>
{
    using HandleType = int; // ハンドルの型

    // リソースを読み込む静的メソッド
    static HandleType Load(const std::wstring& path) {
        return MV1LoadModel(path.c_str());
    }
    // リソースを解放する静的メソッド
    static void Unload(HandleType handle) {
        if (handle != -1) MV1DeleteModel(handle);
    }
};

// ImageTagに対する特殊化
template<>
struct ResourceTraits<ImageTag>
{
    using HandleType = int;

    static HandleType Load(const std::wstring& path) {
        return LoadGraph(path.c_str());
    }
    static void Unload(HandleType handle) {
        if (handle != -1) DeleteGraph(handle);
    }
};

// SoundTagに対する特殊化
template<>
struct ResourceTraits<SoundTag>
{
    using HandleType = int;

    static HandleType Load(const std::wstring& path) {
        return LoadSoundMem(path.c_str());
    }
    static void Unload(HandleType handle) {
        if (handle != -1) DeleteSoundMem(handle);
    }
};