#pragma once
#include <vector>
#include <string>
#include <DxLib.h>

/**
 * @class DebugRenderer
 * @brief デバッグ用の図形描画を管理するシングルトン
 */
class DebugRenderer
{
public:
    static DebugRenderer& GetInstance();

    // コピーとムーブは禁止
    DebugRenderer(const DebugRenderer&) = delete;
    DebugRenderer& operator=(const DebugRenderer&) = delete;

    // --- 描画リクエスト用メソッド ---
    void AddLine(const VECTOR& start, const VECTOR& end, unsigned int color);
    void AddSphere(const VECTOR& center, float radius, unsigned int color);
    void AddString(int x, int y, const std::string& text, unsigned int color);

    // 毎フレームの最後に、リクエストされた図形を全て描画する
    void RenderAll();

private:
    DebugRenderer() = default;

    // 描画リクエストを溜めておくための構造体
    struct LineInfo { VECTOR start, end; unsigned int color; };
    struct SphereInfo { VECTOR center; float radius; unsigned int color; };
    struct StringInfo { int x, y; std::string text; unsigned int color; };

    std::vector<LineInfo> m_lines;
    std::vector<SphereInfo> m_spheres;
    std::vector<StringInfo> m_strings;
};