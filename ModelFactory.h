#pragma once

#include <unordered_map>
#include <string>
#include <DxLib.h>

// モデルリソースを管理するシングルトンクラス
class ModelFactory
{
public:
    // 唯一のインスタンスを取得する
    static ModelFactory& GetInstance()
    {
        static ModelFactory instance; // C++11以降、スレッドセーフな初期化が保証される
        return instance;
    }

    // モデルの読み込み
    int Load(const std::wstring& path);

private:
    // シングルトンのための設定
    ModelFactory() = default;  // コンストラクタ
    ~ModelFactory(); // デストラクタでリソースを解放する

    // コピーとムーブを禁止する
    ModelFactory(const ModelFactory&) = delete;
    ModelFactory& operator=(const ModelFactory&) = delete;
    ModelFactory(ModelFactory&&) = delete;
    ModelFactory& operator=(ModelFactory&&) = delete;

    void Clear(); // 解放処理はプライベートに

    std::unordered_map<std::wstring, int> m_cache;
};