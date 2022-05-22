#pragma once
#include <d3d11.h>
#include <D3Dcompiler.h>

//º¯ÊýÖ¸Õë
typedef HRESULT(__stdcall *tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyscInterval, UINT Flags);
typedef void (__stdcall *tD3D11DrawIndexed)(ID3D11DeviceContext* pContext,UINT IndexCount,UINT StartIndexLocation,INT BaseVertexLocation);

namespace Hooks
{
	extern tD3D11Present oPresent;
	extern tD3D11DrawIndexed oDrawIndexed;
	HRESULT __stdcall hkD3D11present(IDXGISwapChain* pSwapChain, UINT SyscInterval, UINT Flags);
	void __stdcall hkD3D11DrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
}