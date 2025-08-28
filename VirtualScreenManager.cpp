#define NOMINMAX
#include "VirtualScreenManager.h"
#include <DxLib.h>
#include <stdexcept>
#include <algorithm>

// この関数が初めて呼ばれたときに一度だけインスタンスが生成される
VirtualScreenManager& VirtualScreenManager::GetInstance()
{
    static VirtualScreenManager instance;
    return instance;
}

void VirtualScreenManager::Init(int virtualW, int virtualH, ScalingMode mode, unsigned int backgroundColor)
{
    if (m_initialized)
    {
        return; // すでに初期化済み
    }

    if (GetColorBitDepth() == 0)
    {
        throw std::runtime_error("DxLib_Init()より前にVirtualScreenManager::Init()が呼ばれました");
    }

    m_virtualWidth = virtualW;
    m_virtualHeight = virtualH;
    m_scalingMode = mode;
    m_backgroundColor = backgroundColor;

    m_virtualScreenHandle = MakeScreen(m_virtualWidth, m_virtualHeight, TRUE);
    if (m_virtualScreenHandle == -1)
    {
        throw std::runtime_error("仮想画面の作成に失敗しました");
    }

    m_initialized = true;

    // 初期パラメータを計算
    UpdateScalingParameters();
}

void VirtualScreenManager::BeginDraw()
{
    if (!m_initialized) return;

    SetDrawScreen(m_virtualScreenHandle);
    ClearDrawScreen(); // Zバッファなどもクリア
    // DrawBox(0, 0, m_virtualWidth, m_virtualHeight, m_backgroundColor, TRUE);
}

void VirtualScreenManager::EndDraw()
{
    if (!m_initialized) return;

    // 毎フレーム、ウィンドウサイズの変更をチェック
    UpdateScalingParameters();

    // 描画先をバックバッファ（実際の画面）に戻す
    SetDrawScreen(DX_SCREEN_BACK);

    // 計算済みのパラメータを使って仮想スクリーンを実画面に描画
    int drawW = static_cast<int>(m_virtualWidth * m_scaleRatio.x);
    int drawH = static_cast<int>(m_virtualHeight * m_scaleRatio.y);

    DrawExtendGraph(m_drawOffset.x, m_drawOffset.y, m_drawOffset.x + drawW, m_drawOffset.y + drawH, m_virtualScreenHandle, TRUE);
}

void VirtualScreenManager::UpdateScalingParameters()
{
    int currentWindowW, currentWindowH;
    GetWindowSize(&currentWindowW, &currentWindowH);

    // `std::optional`を使い、ウィンドウサイズが変更されたかチェック
    if (m_windowSize.has_value() && m_windowSize->x == currentWindowW && m_windowSize->y == currentWindowH)
    {
        return; // サイズ変更がなければ何もしない
    }

    m_windowSize = Vector2I(currentWindowW, currentWindowH);

    if (m_scalingMode == ScalingMode::StretchToFill)
    {
        m_scaleRatio.x = m_windowSize->x / static_cast<float>(m_virtualWidth);
        m_scaleRatio.y = m_windowSize->y / static_cast<float>(m_virtualHeight);
        m_drawOffset = { 0, 0 };
    }
    else // KeepAspect
    {
        float scale = std::min(m_windowSize->x / static_cast<float>(m_virtualWidth),
            m_windowSize->y / static_cast<float>(m_virtualHeight));
        m_scaleRatio = { scale, scale };

        int drawW = static_cast<int>(m_virtualWidth * scale);
        int drawH = static_cast<int>(m_virtualHeight * scale);
        m_drawOffset.x = (m_windowSize->x - drawW) / 2;
        m_drawOffset.y = (m_windowSize->y - drawH) / 2;
    }
}

Vector2I VirtualScreenManager::ConvertMousePositionToVirtual()
{
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    int relX = mouseX - m_drawOffset.x;
    int relY = mouseY - m_drawOffset.y;

    return Vector2I(
        static_cast<int>(relX / m_scaleRatio.x),
        static_cast<int>(relY / m_scaleRatio.y)
    );
}

Vector2I VirtualScreenManager::ConvertVirtualToScreen(const Vector2I& virtualPos)
{
    return Vector2I(
        static_cast<int>(m_drawOffset.x + virtualPos.x * m_scaleRatio.x),
        static_cast<int>(m_drawOffset.y + virtualPos.y * m_scaleRatio.y)
    );
}

// --- ゲッターの実装 ---
int VirtualScreenManager::GetVirtualWidth() const { return m_virtualWidth; }
int VirtualScreenManager::GetVirtualHeight() const { return m_virtualHeight; }
int VirtualScreenManager::GetVirtualScreenHandle() const { return m_virtualScreenHandle; }
bool VirtualScreenManager::IsInitialized() const { return m_initialized; }
VirtualScreenManager::ScalingMode VirtualScreenManager::GetScalingMode() const { return m_scalingMode; }