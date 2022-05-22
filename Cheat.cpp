#include "stdafx.h"
#include "Cheat.h"
#include <iostream>
#include "Helpers.h"
#include "Hooks.h"
#include "Renderer.h"


ID3D11Device* pDevice;
ID3D11DeviceContext * pContext;
IDXGISwapChain * pSwapChain;
ID3D11RenderTargetView* pRenderTargetView;

CCheat::CCheat()
{
}

CCheat::~CCheat()
{
}

void CCheat::Initialize()
{
	AllocConsole();
	freopen("CON", "w", stdout);
	SetConsoleTitleA("ChearDLL");
	HWND hWindow =FindWindow(NULL,"萤火突击");//获取窗口句柄
	printf("窗口句柄：%X\n", hWindow);
	//交换链
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd,0,sizeof(scd));//填充0
	scd.BufferCount = 1;						//我们只创建一个后缓冲(双缓冲)因此为1
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//设置颜色格式，rgb-a

	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//缩放比
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//扫描线
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//目标的输出
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//容许模式切换
	scd.OutputWindow = hWindow;//在游戏窗体内绘制
	scd.SampleDesc.Count = 1;					//1重采样
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//常用参数
	scd.Windowed = ((GetWindowLongPtr(hWindow, GWL_STYLE)&WS_POPUP) != 0) ? false:true;	//是否全屏
	scd.BufferDesc.Width = 1920;
	scd.BufferDesc.Height = 1080;
	scd.BufferDesc.RefreshRate.Numerator = 40;//帧率
	scd.BufferDesc.RefreshRate.Denominator = 1;//分母
	scd.SampleDesc.Quality = 0;					//采样等级
	

	D3D_FEATURE_LEVEL featrueLevel = D3D_FEATURE_LEVEL_11_0;
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		&featrueLevel,
		1,
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		NULL,
		&pContext);

	DWORD_PTR* pSwapChainVT = NULL;
	DWORD_PTR* pDeviceVT = NULL;
	DWORD_PTR* pContextVT = NULL;

	printf("pSwapChain:%X\n",pSwapChain);
	printf("pDevice:%X\n",pDevice);
	printf("pContext:%X\n",pContext);

	pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChain);
	pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDevice);
	pContextVT = reinterpret_cast<DWORD_PTR*>(pContext);

	pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChainVT[0]);//重要
	pContextVT = reinterpret_cast<DWORD_PTR*>(pContextVT[0]);//重要

	Hooks::oPresent=reinterpret_cast<tD3D11Present>(pSwapChainVT[8]);
	Hooks::oDrawIndexed= reinterpret_cast<tD3D11DrawIndexed>(pContextVT[12]);

	Helpers::HookFunction(reinterpret_cast<PVOID *>(&Hooks::oPresent), Hooks::hkD3D11present);
	Helpers::HookFunction(reinterpret_cast<PVOID *>(&Hooks::oDrawIndexed), Hooks::hkD3D11DrawIndexed);


	printf("oDrawIndexed:%X\n", pContextVT[12]);


}

void CCheat::Release()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	FreeConsole();

	Helpers::UnHookFunction(reinterpret_cast<PVOID *>(&Hooks::oPresent), Hooks::hkD3D11present);
	Helpers::UnHookFunction(reinterpret_cast<PVOID *>(&Hooks::oDrawIndexed), Hooks::hkD3D11DrawIndexed);
}