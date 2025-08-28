#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "ResourceTraits.h"
#include <utility>
#include <DxLib.h>

// --- SpriteRendererの実装 ---

SpriteRenderer::SpriteRenderer()
    : m_handle(-1), m_originalWidth(0), m_originalHeight(0),
    m_visible(true), m_color(ColorUtil::White), m_blendParam(255)
{
}

SpriteRenderer::SpriteRenderer(const std::wstring& path) : SpriteRenderer() { Load(path); }
SpriteRenderer::~SpriteRenderer() { Release(); }

// ムーブコンストラクタ
SpriteRenderer::SpriteRenderer(SpriteRenderer&& other) noexcept
    : m_handle(other.m_handle), m_originalWidth(other.m_originalWidth), m_originalHeight(other.m_originalHeight),
    m_visible(other.m_visible), m_color(other.m_color), m_blendParam(other.m_blendParam)
{
    other.m_handle = -1; // 所有権をムーブしたので、相手は無効化
}

// ムーブ代入演算子
SpriteRenderer& SpriteRenderer::operator=(SpriteRenderer&& other) noexcept
{
    if (this != &other)
    {
        Release();
        m_handle = other.m_handle;
        m_originalWidth = other.m_originalWidth;
        m_originalHeight = other.m_originalHeight;
        m_visible = other.m_visible;
        m_color = other.m_color;
        m_blendParam = other.m_blendParam;

        other.m_handle = -1;
    }
    return *this;
}


bool SpriteRenderer::Load(const std::wstring& path)
{
    Release();
    m_handle = ResourceManager::Instance().Get<ImageTag>(path);
    if (m_handle != -1)
    {
        GetGraphSize(m_handle, &m_originalWidth, &m_originalHeight);
        return true;
    }
    return false;
}

void SpriteRenderer::Draw(const Transform2D& transform) const
{
    if (!m_visible || m_handle == -1) return;

    // ★ 描画設定は行わない！ UISystemに一任する

    // 色の変更だけは個別に行う
    SetDrawBright(ColorUtil::GetR(m_color), ColorUtil::GetG(m_color), ColorUtil::GetB(m_color));

    // ブレンドの強さを設定
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendParam);

    const VECTOR2 pos = transform.GetPosition();
    const VECTOR2 scale = transform.GetScale();
    const float rotation = transform.GetRotation();
    const VECTOR2 pivot = transform.GetPivot();

    const float centerX = static_cast<float>(m_originalWidth * pivot.x);
    const float centerY = static_cast<float>(m_originalHeight * pivot.y);

    DrawRotaGraph3F(
        pos.x, pos.y,
        centerX, centerY,
        scale.x,
        scale.y,
        static_cast<double>(rotation),
        m_handle,
        TRUE, // 透明有効
        FALSE
    );

    // 描画後に色を白に戻す
    SetDrawBright(255, 255, 255);
}

// --- セッターとゲッターの実装 ---
void SpriteRenderer::SetVisible(bool visible) { m_visible = visible; }
void SpriteRenderer::SetColor(unsigned int color) { m_color = color; }
void SpriteRenderer::SetBlendParam(int blendParam) { m_blendParam = blendParam; }
int SpriteRenderer::GetOriginalWidth() const { return m_originalWidth; }
int SpriteRenderer::GetOriginalHeight() const { return m_originalHeight; }
int SpriteRenderer::GetHandle() const { return m_handle; }

void SpriteRenderer::Release()
{
    if (m_handle != -1)
    {
        m_handle = -1;
        m_originalWidth = 0;
        m_originalHeight = 0;
    }
}