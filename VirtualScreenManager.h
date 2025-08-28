#pragma once
#include "Vector.h" // テンプレート化したVector.hをインクルード
#include <optional>   // std::optional を使用

// 仮想スクリーンの管理クラス
class VirtualScreenManager
{
public:
    enum class ScalingMode
    {
        StretchToFill,
        KeepAspect
    };

    // 唯一のインスタンスを取得するための静的メソッド
    static VirtualScreenManager& GetInstance();

    // シングルトンパターンのお作法として、コピーとムーブを禁止
    VirtualScreenManager(const VirtualScreenManager&) = delete;
    VirtualScreenManager& operator=(const VirtualScreenManager&) = delete;

    void Init(int virtualW, int virtualH, ScalingMode mode = ScalingMode::KeepAspect, unsigned int backgroundColor = 0x00FFFF);
    void BeginDraw();
    void EndDraw();

    Vector2I ConvertMousePositionToVirtual();
    Vector2I ConvertVirtualToScreen(const Vector2I& virtualPos);

    // ゲッター
    int GetVirtualWidth() const;
    int GetVirtualHeight() const;
    int GetVirtualScreenHandle() const;
    bool IsInitialized() const;
    ScalingMode GetScalingMode() const;

private:
    // privateコンストラクタで、外部からの直接的なインスタンス化を禁止
    VirtualScreenManager() = default;

    void UpdateScalingParameters();

    // メンバ変数
    int m_virtualWidth = 0;
    int m_virtualHeight = 0;
    int m_virtualScreenHandle = -1;
    bool m_initialized = false;
    ScalingMode m_scalingMode = ScalingMode::KeepAspect;
    unsigned int m_backgroundColor = 0x00FFFF;

    // `std::optional`で「値がまだない」状態を安全に表現
    std::optional<Vector2I> m_windowSize;

    Vector2I m_drawOffset = { 0, 0 };
    Vector2F m_scaleRatio = { 1.0f, 1.0f };
};