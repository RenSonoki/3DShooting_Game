#include "Random.h"

Random& Random::GetInstance()
{
    static Random instance;
    return instance;
}

// privateコンストラクタ内で、最初のインスタンス生成時に一度だけシード
Random::Random()
{
    std::random_device rd;
    m_engine.seed(rd());
}

int Random::GetInt(int min, int max)
{
    // m_mutexがこのスコープの間、m_engineへのアクセスを保護する
	// NOTE: thread_localを使ってスレッドごとに乱数生成器を持つ方法もある
    std::lock_guard<std::mutex> lock(m_mutex);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(m_engine);
}

float Random::GetFloat(float min, float max)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::uniform_real_distribution<float> dist(min, max);
    return dist(m_engine);
}