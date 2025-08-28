#pragma once
#include "ComponentBase.h"
#include <string>
#include <memory>

class TransformComponent;

class RenderModelComponent : public ComponentBase
{
public:
    RenderModelComponent();
    virtual ~RenderModelComponent();

    void Start() override;
    void Draw() const override;
    ComponentID GetID() const override;

    bool SetModel(const std::wstring& modelPath);
    int GetModelHandle() const;

private:
    std::wstring m_modelPath;
    int m_modelHandle;
    std::weak_ptr<TransformComponent> m_transform;
};

//#pragma once
//#include "ComponentBase.h"
//#include <string>
//#include <memory>
//
//class TransformComponent;
//
//class RenderModelComponent : public ComponentBase
//{
//public:
//    // ★ 修正点: デフォルトコンストラクタに変更
//    RenderModelComponent();
//    // ★ 修正点: モデル解放のためのデストラクタを追加
//    virtual ~RenderModelComponent();
//
//    void Start() override;
//    void Draw() const override;
//    ComponentID GetID() const override;
//
//    // モデルを設定する唯一のインターフェース
//    bool SetModel(const std::wstring& modelPath);
//    int GetModelHandle() const;
//
//private:
//    std::wstring m_modelPath;
//    int m_modelHandle;
//    std::weak_ptr<TransformComponent> m_transform;
//};