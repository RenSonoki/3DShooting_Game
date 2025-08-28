#pragma once
#include <random>
#include <mutex>

// ランダム数生成クラス
class Random
{
public:
    static Random& GetInstance();

	// コピーとムーブを禁止(シングルトンパターンのため)
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    // 指定範囲の整数を生成 (スレッドセーフ)
    int GetInt(int min, int max);

    // 指定範囲の浮動小数点数を生成 (スレッドセーフ)
    float GetFloat(float min, float max);

private:
    // コンストラクタでシードを初期化
    Random();

    std::mt19937 m_engine;
    std::mutex m_mutex;
};