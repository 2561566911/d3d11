#pragma once
#include <d3d11.h>
#include <D3Dcompiler.h>

extern ID3D11Device* pDevice;
extern ID3D11DeviceContext * pContext;
extern IDXGISwapChain * pSwapChain;

extern ID3D11RenderTargetView* pRenderTargetView;

class CCheat
{
public:
	CCheat();
	~CCheat();

	void Initialize();
	void Release();

	
};

