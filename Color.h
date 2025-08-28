#pragma once

// 色を扱うためのヘルパー関数群を定義
namespace ColorUtil
{
    // C++17以降ではインライン変数としてヘッダに直接定義できます
    inline constexpr unsigned int White = 0xFFFFFFFF;
    inline constexpr unsigned int Black = 0x00000000;
    inline constexpr unsigned int Red = 0xFFFF0000;
    inline constexpr unsigned int Green = 0xFF00FF00;
    inline constexpr unsigned int Blue = 0xFF0000FF;

    // unsigned int のカラー値からR成分(0-255)を抽出する
    inline constexpr unsigned char GetR(unsigned int color)
    {
        return static_cast<unsigned char>((color >> 16) & 0xFF);
    }

    // G成分(0-255)を抽出する
    inline constexpr unsigned char GetG(unsigned int color)
    {
        return static_cast<unsigned char>((color >> 8) & 0xFF);
    }

    // B成分(0-255)を抽出する
    inline constexpr unsigned char GetB(unsigned int color)
    {
        return static_cast<unsigned char>(color & 0xFF);
    }
}