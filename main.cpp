
#include "main.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "field.h"
#include "grid.h"
//#include "debugproc.h"
#include "player.h"
#include "billboard.h"
#include "shadow.h"
#include "bullet.h"
#include "box.h"
#include "wallmap.h"
#include "wallB.h"
#include "wallL.h"
#include "wallR.h"
#include "enemy.h"
#include "collision.h"
#include <time.h>


#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "winmm.lib")


/*------------------------------------------------------------------------------
定数定義
------------------------------------------------------------------------------*/
#define CLASS_NAME     "GameWindow"       // ウインドウクラスの名前
#define WINDOW_CAPTION "ゲームウィンドウ" // ウィンドウの名前


/*------------------------------------------------------------------------------
   グローバル変数宣言
------------------------------------------------------------------------------*/
static HWND g_hWnd;                 // ウィンドウハンドル

#ifdef _DEBUG
int					g_nCountFPS;			// FPSカウンタ
#endif
bool				g_bDispDebug = true;	// デバッグ表示ON/OFF

/*------------------------------------------------------------------------------
プロトタイプ宣言
------------------------------------------------------------------------------*/
// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ゲームの初期化
static bool Initialize(HINSTANCE hInst);
static void Finalize(void);

// ゲームの更新関数
static void Update(void);
// ゲームの描画関数
static void Draw(void);

//フラグ　処理を1回だけするため
static bool Flag = false;
int ran;
/*------------------------------------------------------------------------------
メイン
------------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 使用しない一時変数を明示
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ウィンドウクラス構造体の設定
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;                          // ウィンドウプロシージャの指定
	wc.lpszClassName = CLASS_NAME;                     // クラス名の設定
	wc.hInstance = hInstance;                          // インスタンスハンドルの指定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // マウスカーソルを指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ウインドウのクライアント領域の背景色を設定

	// クラス登録
	RegisterClass(&wc);


	// ウィンドウスタイル
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~( WS_THICKFRAME);

	// 基本矩形座標
	RECT window_rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// 指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);

	// 新たなWindowの矩形座標から幅と高さを算出
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	// プライマリモニターの画面解像度取得
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	// デスクトップの真ん中にウィンドウが生成されるように座標を計算
	// ※ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	// ウィンドウの生成
	g_hWnd = CreateWindow(
		CLASS_NAME,     // ウィンドウクラス
		WINDOW_CAPTION, // ウィンドウテキスト
		window_style,   // ウィンドウスタイル
		window_x,       // ウィンドウ座標x
		window_y,       // ウィンドウ座標y
		window_width,   // ウィンドウの幅
		window_height,  // ウィンドウの高さ
		NULL,           // 親ウィンドウハンドル
		NULL,           // メニューハンドル
		hInstance,      // インスタンスハンドル
		NULL            // 追加のアプリケーションデータ
		);

	if (g_hWnd == NULL) {
		// ウィンドウハンドルが何らかの理由で生成出来なかった
		return -1;
	}

	// 指定のウィンドウハンドルのウィンドウを指定の方法で表示
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// ゲームの初期化
	Initialize(hInstance);


	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwExecLastTime = dwFPSLastTime = timeGetTime();
	dwCurrentTime = dwFrameCount = 0;

	// Windowsゲーム用メインループ
	MSG msg = {}; // msg.message == WM_NULL
	while (WM_QUIT != msg.message) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// メッセージがある場合はメッセージ処理を優先
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
//				PrintDebugProc("FPS:%d\n", g_nCountFPS);
#endif
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}

	// ゲームの終了処理
	Finalize();

	return (int)msg.wParam;
}

// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSEメッセージの送信
		}
		break;

	case WM_CLOSE:
//		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(hWnd); // 指定のウィンドウにWM_DESTROYメッセージを送る
//		}
		return 0; // DefWindowProc関数にメッセージを流さず終了することによって何もなかったことにする

	case WM_DESTROY: // ウィンドウの破棄メッセージ
		PostQuitMessage(0); // WM_QUITメッセージの送信
		return 0;
	};

	// 通常メッセージ処理はこの関数に任せる
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Initialize(HINSTANCE hInst)
{
	//シードの初期化
	srand((unsigned int)time(NULL));

	// ゲームの初期化(Direct3Dの初期化)
	if (!D3D_Initialize(g_hWnd)){
		// ゲームの初期化に失敗した
		return false;
	}
	// DirectInputの初期化（キーボード）
	if (!Keyboard_Initialize(hInst, g_hWnd)){
		return false;
	}
	// DirectInputの初期化（ゲームパッド）
	if (!GamePad_Initialize(hInst, g_hWnd)){
		return false;
	}

	//全テクスチャファイルのロード
	Texture_Load();

	// デバッグ表示処理 
	//DebugProc_Initialize();

	// カメラの初期化処理
	Camera_Initialize();

	// ライトの初期化処理
	Light_Initialize();

	// 地面の初期化処理
	Field_Initialize();

	//wall
	Wall_Initialize();
	WallL_Initialize();
	WallB_Initialize();
	WallR_Initialize();
	//影の初期化
	Shadow_Initialize();//使用する処理より先に初期化しないとダメ

	//グリッドの初期化
//	Grid_Initialize();
	
    //billboard初期化
	Billboard_Initialize();

	//プレイヤーの初期化
	Player_Initialize(D3DXVECTOR3(500, 100, -600), D3DXVECTOR3(0, 0, 0));

	//敵の初期化
	Enemy_Initialize();

	//弾丸の初期化
	Bullet_Initialize();

	//
	

	return true;
}

void Finalize(void)
{
	// カメラの終了処理
	Camera_Finalize();

	// ライトの終了処理
	Light_Finalize();

	// 地面の終了処理
	Field_Finalize();

	Wall_Finalize();
	WallB_Finalize();
	WallR_Finalize();
	WallL_Finalize();

	//box
	//box_Finalize();

	//グリッドの終了処理
//	Grid_Finalize();
	
	//終了処理
	Billboard_Finalize();

	//影の終了処理
	Shadow_Finalize();

	//敵の終了処理
	Enemy_Finalize();

	//プレイヤーの終了処理
	Player_Finalize();

	//弾丸の終了処理
	Bullet_Finalize();

	
	// テクスチャの解放
	Texture_Release();


	// DirectInputの終了処理
	GamePad_Finalize();

	// DirectInputの終了処理
	Keyboard_Finalize();

	// ゲームの終了処理(Direct3Dの終了処理)
	D3D_Finalize();
}

// ゲームの更新関数
void Update(void)
{
	//キーボードの状態を更新する
	Keyboard_Update();

	//ゲームパッドの状態を更新する
	GamePad_Update();

	// カメラの更新処理
	Camera_Update();

	// ライトの更新処理
	Light_Update();

	// 地面の更新処理
	Field_Update();

	//box
	//box_Update();

	//グリッドの更新処理
//	Grid_Update();

	//
	Shadow_Update();

	//更新処理
	Billboard_Update();

	//プレイヤーの更新処理
	Player_Update();

	//敵の更新処理
	Enemy_Update();

	//弾丸の更新処理
	Bullet_Update();

	//
	

	//当たり判定
	Collision_Update();

}

// ゲームの描画関数
void Draw(void)
{
	LPDIRECT3DDEVICE9 pD3DDevice = GetD3DDevice();

	// 画面のクリア
	pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(50, 50, 50, 255), 1.0f, 0);

	// 描画バッチ命令の開始
	pD3DDevice->BeginScene();


	// カメラの設定
	Camera_SetCamera();

	// 地面の描画処理
	Field_Draw();

	
	//壁の描画
	
    Wall_Draw(); 
	WallL_Draw();
	WallB_Draw();
	WallR_Draw();
	//グリッドの描画処理
    //Grid_Draw();

	//
	Shadow_Draw();

	//描画処理
	Billboard_Draw();

	//プレイヤーの描画処理
	Player_Draw();

	//敵の描画処理
	Enemy_Draw();

	//弾丸の描画処理
	Bullet_Draw();

	//
	

	// デバッグ表示の描画処理
	if (g_bDispDebug)
	{
		//DebugProc_Draw();
	}

	// 描画バッチ命令の終了
	pD3DDevice->EndScene();

	// バックバッファをフリップ（タイミングはD3DPRESENT_PARAMETERSの設定による）
	pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//ウィンドウハンドルのゲッター
HWND GetHWND(){
	return g_hWnd;
}


double frand() {
	return (double)rand() / RAND_MAX;
}
