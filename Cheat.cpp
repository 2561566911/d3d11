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
	HWND hWindow =FindWindow(NULL,"ө��ͻ��");//��ȡ���ھ��
	printf("���ھ����%X\n", hWindow);
	//������
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd,0,sizeof(scd));//���0
	scd.BufferCount = 1;						//����ֻ����һ���󻺳�(˫����)���Ϊ1
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//������ɫ��ʽ��rgb-a

	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;//���ű�
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;//ɨ����
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//Ŀ������
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//����ģʽ�л�
	scd.OutputWindow = hWindow;//����Ϸ�����ڻ���
	scd.SampleDesc.Count = 1;					//1�ز���
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	//���ò���
	scd.Windowed = ((GetWindowLongPtr(hWindow, GWL_STYLE)&WS_POPUP) != 0) ? false:true;	//�Ƿ�ȫ��
	scd.BufferDesc.Width = 1920;
	scd.BufferDesc.Height = 1080;
	scd.BufferDesc.RefreshRate.Numerator = 40;//֡��
	scd.BufferDesc.RefreshRate.Denominator = 1;//��ĸ
	scd.SampleDesc.Quality = 0;					//�����ȼ�
	

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

	pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChainVT[0]);//��Ҫ
	pContextVT = reinterpret_cast<DWORD_PTR*>(pContextVT[0]);//��Ҫ

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