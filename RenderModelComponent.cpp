#include "RenderModelComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <DxLib.h>
#include <cassert>

RenderModelComponent::RenderModelComponent()
    : m_modelPath(L"")
    , m_modelHandle(-1)
{
}

RenderModelComponent::~RenderModelComponent()
{
    if (m_modelHandle != -1)
    {
        MV1DeleteModel(m_modelHandle);
        m_modelHandle = -1;
    }
}

void RenderModelComponent::Start()
{
    m_transform = GetOwner()->GetComponent<TransformComponent>();
    assert(!m_transform.expired() && "RenderModelComponent requires a TransformComponent.");
}

ComponentID RenderModelComponent::GetID() const
{
    return ComponentID::ModelRenderer;
}

bool RenderModelComponent::SetModel(const std::wstring& modelPath)
{
    if (m_modelHandle != -1)
    {
        MV1DeleteModel(m_modelHandle);
    }

    m_modelPath = modelPath;
    m_modelHandle = MV1LoadModel(modelPath.c_str());

    if (m_modelHandle != -1)
    {
        // アニメーションによるルートモーションを無効化
        int centerFrame = MV1SearchFrame(m_modelHandle, L"センター");
        if (centerFrame != -1)
        {
            MV1SetFrameUserLocalMatrix(m_modelHandle, centerFrame, MGetIdent());
        }
    }

    return m_modelHandle != -1;
}

int RenderModelComponent::GetModelHandle() const
{
    return m_modelHandle;
}

void RenderModelComponent::Draw() const
{
    auto transform = m_transform.lock();
    if (m_modelHandle == -1 || !transform)
    {
        return;
    }

    // MV1DrawModelを呼び出す「前」に、そのモデルにアタッチされている
    // 全てのアニメーションの再生状態を、モデルの頂点や当たり判定に反映させる命令です。
    // これがないと、ModelAnimatorComponentがいくら時間を更新しても、見た目が変わりません。
    MV1RefreshCollInfo(m_modelHandle, -1);

    // 2. アニメーションをモデルの見た目（頂点）に反映させる
    //    これが、アニメーションが動いて見えるようにするための鍵です。
    // MV1RefreshVertBuffer(m_modelHandle);
    // ★★★ ここまで ★★★

    // ワールド行列の計算と設定
    MATRIX worldMat = transform->GetWorldMatrix();
    MV1SetMatrix(m_modelHandle, worldMat);

    // 更新された状態で、モデルを描画
    MV1DrawModel(m_modelHandle);
}

//#include "RenderModelComponent.h"
//#include "Entity.h"
//#include "TransformComponent.h"
//#include <DxLib.h>
//#include <cassert>
//
//// ★ 修正点: デフォルトコンストラクタの実装
//RenderModelComponent::RenderModelComponent()
//    : m_modelPath(L"")
//    , m_modelHandle(-1)
//{
//}
//
//// ★ 修正点: デストラクタでモデルを解放し、メモリリークを防ぐ
//RenderModelComponent::~RenderModelComponent()
//{
//    if (m_modelHandle != -1)
//    {
//        MV1DeleteModel(m_modelHandle);
//        m_modelHandle = -1;
//    }
//}
//
//void RenderModelComponent::Start()
//{
//    // Startの責務は、他のコンポーネントへの参照を解決すること
//    m_transform = GetOwner()->GetComponent<TransformComponent>();
//    assert(!m_transform.expired() && "RenderModelComponent requires a TransformComponent.");
//}
//
//ComponentID RenderModelComponent::GetID() const
//{
//    return ComponentID::ModelRenderer;
//}
//
//bool RenderModelComponent::SetModel(const std::wstring& modelPath)
//{
//    // 既に別のモデルを読み込んでいれば、先に解放する
//    if (m_modelHandle != -1)
//    {
//        MV1DeleteModel(m_modelHandle);
//    }
//
//    m_modelPath = modelPath;
//    m_modelHandle = MV1LoadModel(modelPath.c_str());
//
//    // 💡 ★★★ 最も重要な修正点 ★★★
//    // モデルの読み込みに成功した場合...
//    if (m_modelHandle != -1)
//    {
//        // アニメーションによるルートモーション（特にMMDモデルの「センター」ボーンの移動）を
//        // 無効化し、移動は全て MovementSystem に一任します。
//        // これをしないと、アニメーションとプログラムが二重にキャラクターを動かしてしまいます。
//        int centerFrame = MV1SearchFrame(m_modelHandle, L"センター");
//        if (centerFrame != -1)
//        {
//            MV1SetFrameUserLocalMatrix(m_modelHandle, centerFrame, MGetIdent());
//        }
//    }
//    // ★★★ ここまで ★★★
//
//    return m_modelHandle != -1;
//}
//
//int RenderModelComponent::GetModelHandle() const
//{
//    return m_modelHandle;
//}
//
//void RenderModelComponent::Draw() const
//{
//    auto transform = m_transform.lock();
//    if (m_modelHandle == -1 || !transform)
//    {
//        return;
//    }
//
//    MV1RefreshCollInfo(m_modelHandle, -1);
//
//    // ワールド行列の計算と設定 (変更なし)
//    MATRIX worldMat = transform->GetWorldMatrix();
//    MV1SetMatrix(m_modelHandle, worldMat);
//
//    // モデルを描画
//    MV1DrawModel(m_modelHandle);
//}