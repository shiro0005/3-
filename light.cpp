
#include "light.h"
#include "myDirect3D.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_LIGHT		(4)		// ライトの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DLIGHT9 g_aLight[NUM_LIGHT];		// ライト情報
static int cnt;
static bool flag;
//=============================================================================
// ライトの初期化処理
//=============================================================================
void Light_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 
	D3DXVECTOR3 vecDir;

	cnt = 0;
	flag = false;

	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[0], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 環境光
	g_aLight[0].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.20f, -0.80f, 0.40f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(0, &g_aLight[0]);

	// ライトを有効に
	pDevice->LightEnable(0,TRUE);//ここが全体の光





	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[1], sizeof(D3DLIGHT9));

	// ライトのタイプの設定
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// 拡散光
	g_aLight[1].Diffuse = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

	// 環境光
	g_aLight[1].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

    // ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.20f, 1.00f, -0.50f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// ライトをレンダリングパイプラインに設定
	pDevice->SetLight(1, &g_aLight[1]);

	// ライトを有効に
	pDevice->LightEnable(1, TRUE);


	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[2], sizeof(D3DLIGHT9));

	// ライト2のタイプの設定
	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	// ライト2の拡散光の設定
	g_aLight[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ライト2の鏡面反射光の設定
	g_aLight[2].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

    // ライト2の方向の設定
	vecDir = D3DXVECTOR3(0.80f, 0.10f, 0.40f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[2].Direction, &vecDir);

	// ライト2をレンダリングパイプラインに設定
	pDevice->SetLight(2, &g_aLight[2]);

	// ライト2を有効に
	pDevice->LightEnable(2, FALSE);


	// D3DLIGHT9構造体を0でクリアする
	ZeroMemory(&g_aLight[3], sizeof(D3DLIGHT9));

	// ライト3のタイプの設定
	g_aLight[3].Type = D3DLIGHT_SPOT;

	//ライト3の位置の設定
	g_aLight[3].Position = D3DXVECTOR3(500, 101, -600);

	// ライト3の拡散光の設定
	//g_aLight[3].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	//ライトの色
	g_aLight[3].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);



	g_aLight[3].Direction= D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	////ライト3の範囲の設定
	g_aLight[3].Range = 100.0f;

	//0～1.0fまで設定　
	g_aLight[3].Falloff = 1.0f;

	//減衰率
	g_aLight[3].Attenuation0 = 0.9f;
	g_aLight[3].Attenuation1 = 0.0f;
	g_aLight[3].Attenuation2 = 0.0f;

	//内側と外側の角度
	g_aLight[3].Theta = 1.1f;//内側
	g_aLight[3].Phi = 2.21f;//外側



	// ライト3の鏡面反射光の設定
	//g_aLight[3].Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);

	// ライト3の方向の設定
	/*vecDir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_aLight[3].Direction, &vecDir);*/

	// ライト3をレンダリングパイプラインに設定
	pDevice->SetLight(3, &g_aLight[3]);



	// ライト3を有効に
	pDevice->LightEnable(3, FALSE);



	// ライティングモード有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);//ライティングするTRUEしないFALSE
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void Light_Finalize(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void Light_Update(void)
{
	PLAYER player = GetPlayer();
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	g_aLight[3].Position = player.posModel;
	g_aLight[3].Position.y = player.posModel.y+20.0f;
	// ライト3をレンダリングパイプラインに設定
	pDevice->SetLight(3, &g_aLight[3]);

	cnt++;
	if (cnt >= 120) {
		LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
		if (!flag) {
			pDevice->LightEnable(0, FALSE);//ここが全体の光
			pDevice->LightEnable(1, FALSE);//ここが全体の光

			g_aLight[3].Position = player.posModel;
			g_aLight[3].Position.y = player.posModel.y + 20.0f;
			// ライト3をレンダリングパイプラインに設定
			pDevice->SetLight(3, &g_aLight[3]);
			pDevice->LightEnable(3, TRUE);//ここが全体の光

			flag = true;
		}
		else {
			pDevice->LightEnable(0, TRUE);//ここが全体の光
			pDevice->LightEnable(1, TRUE);//ここが全体の光

			pDevice->LightEnable(3, FALSE);//ここが全体の光
			flag = false;
		}
		cnt = 0;
	}
}

