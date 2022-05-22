#include "stdafx.h"
#include "Hooks.h"
#include "Cheat.h"

namespace Hooks
{
	tD3D11Present oPresent =NULL;
	tD3D11DrawIndexed oDrawIndexed = NULL;
	
	//全局变量初始化
	bool bOnce = false;
	INT g_inByteWidth = 0;
	INT test_value = 0;
	INT Pcmd = 0;
	INT intP = 2;
	INT f1 = false;
	INT f1P = 2;
	INT f2 = false;
	INT f2P = 2;


	//vertex 顶点 vedesc
	ID3D11Buffer* veBuffer;
	UINT Stride = 0;
	UINT veBufferOffset = 0;
	D3D11_BUFFER_DESC vedesc;
	//

	//indesc 索引
	ID3D11Buffer* inBuffer;
	DXGI_FORMAT inFormat;
	UINT inOffset;
	D3D11_BUFFER_DESC indesc;
	//
	ID3D11RasterizerState * rwState;
	ID3D11RasterizerState * rsState;
	ID3D11ShaderResourceView* ShaderResourceView;

	
	
	/////////////////////////////////
	enum eDepthState{
		ENABLED,
		DISABLED,
		READ_NO_WRITE,
		NO_READ_NO_WRITE,
		_DEPTH_COUNT,
	};
	
	
	ID3D11DepthStencilState* myDepthStencilState[static_cast<int>(eDepthState::_DEPTH_COUNT)];
	//ID3D11DepthStencilState *depthStencilState;
	//ID3D11DepthStencilState *depthStencilStateDisable;

	ID3D11PixelShader* psRed = NULL;
	ID3D11PixelShader* psBlue = NULL;
	ID3D11PixelShader* psQ = NULL;
	ID3D11PixelShader* psH = NULL;
	//----------------------------------
	

	HRESULT GenerateShader(ID3D11Device* pD3DDevice, 
							ID3D11PixelShader** pShader, 
							float r, 
							float g, 
							float b)
	{
		char szCast[] = "struct VS_OUT"
			"{"
			"float4 Position : SV_Position;"
			"float4 Color : COLORO;"
			"};"

			"float4 main(VS_OUT input) : SV_Target"
			"{"
			"float4 fake;"
			"fake.a = 1.0f;"
			"fake.r = %f;"
			"fake.g = %f;"
			"fake.b = %f;"
			"return fake;"
			"}";

		ID3D10Blob* pBlob;
		char szPixelShader[1000];

		sprintf(szPixelShader, szCast, r, g, b);

		ID3DBlob* d3dErrorMsgBlob;

		HRESULT hr = D3DCompile(szPixelShader, sizeof(szPixelShader), "shader", NULL, NULL, "main", "ps_4_0", NULL, NULL, &pBlob, &d3dErrorMsgBlob);

		if (FAILED(hr))
			return hr;

		hr = pD3DDevice->CreatePixelShader((DWORD*)pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, pShader);
		if (FAILED(hr))
			return hr;

		return S_OK;
	}

	HRESULT __stdcall hkD3D11present(IDXGISwapChain* pSwapChain, UINT SyscInterval, UINT Flags)
	{
		
		if (!bOnce)
		{
			//第一次进来
			//得到游戏窗体
			HWND hWindow = FindWindow(NULL, "萤火突击");
			HRESULT hresult = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pDevice);
			if (SUCCEEDED(hresult)){
				pSwapChain->GetDevice(__uuidof(pDevice),(void **)(&pDevice));
				pDevice->GetImmediateContext(&pContext);
			}
			else
				goto endp;

			//////////////////
			D3D11_DEPTH_STENCIL_DESC stencilDesc;
			stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
			stencilDesc.StencilEnable = true;//关键
			stencilDesc.StencilReadMask = 0xFF;
			stencilDesc.StencilWriteMask = 0xFF;
			stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			//EBABLED
			stencilDesc.DepthEnable = true;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilState[static_cast<int>(eDepthState::ENABLED)]);

			//DISABLED
			stencilDesc.DepthEnable = false;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilState[static_cast<int>(eDepthState::DISABLED)]);

			//
			//NO_READ_NO_WRITE
			stencilDesc.DepthEnable = false;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			stencilDesc.StencilEnable = false;
			stencilDesc.StencilReadMask = (UINT8)(0xFF);
			stencilDesc.StencilWriteMask = 0x0;
			pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilState[static_cast<int>(eDepthState::NO_READ_NO_WRITE)]);

			//READ_NO_WRITE
			stencilDesc.DepthEnable = true;
			stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
			stencilDesc.StencilEnable = false;
			stencilDesc.StencilReadMask = (UINT8)(0xFF);
			stencilDesc.StencilWriteMask = 0x0;
			stencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
			stencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
			stencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
			pDevice->CreateDepthStencilState(&stencilDesc, &myDepthStencilState[static_cast<int>(eDepthState::READ_NO_WRITE)]);

			D3D11_RASTERIZER_DESC rwDesc;
			pContext->RSGetState(&rwState);
			rwState->GetDesc(&rwDesc);
			rwDesc.FillMode = D3D11_FILL_WIREFRAME;
			rwDesc.CullMode = D3D11_CULL_NONE;
			pDevice->CreateRasterizerState(&rwDesc, &rwState);

			D3D11_RASTERIZER_DESC rsDesc;
			pContext->RSGetState(&rsState);
			rsState->GetDesc(&rsDesc);
			rsDesc.FillMode = D3D11_FILL_SOLID;
			rsDesc.CullMode = D3D11_CULL_NONE;
			pDevice->CreateRasterizerState(&rsDesc, &rsState);


			//红
			GenerateShader(pDevice, &psRed, 1.0f, 0.0f, 0.0f);
			//蓝
			GenerateShader(pDevice, &psBlue, 0.0f, 0.0f, 1.0f);
			//黄
			GenerateShader(pDevice, &psH, 1.0f, 1.0f, 0.0f);
			//青
			GenerateShader(pDevice, &psQ, 0.0f, 1.0f, 1.0f);

			//------------------------------

			bOnce = true;
		}
			
		pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	endp:
		return oPresent(pSwapChain, SyscInterval, Flags);
	}




	void SetDepthStencilState(eDepthState aState){
		pContext->OMSetDepthStencilState(myDepthStencilState[aState],1);
	}
	/*void SetDepthStencilStateg(eDepthState aState){
		pContext->OMSetDepthStencilState(myDepthStencilState[aState], 0);
	}*/

	void __stdcall Hooks::hkD3D11DrawIndexed(ID3D11DeviceContext* pContext,
		UINT IndexCount,
		UINT StartIndexLocation,
		INT BaseVertexLocation)
	{
		if (!psRed || !psBlue)
			goto end;

		//------------------------------模型字节函数-------------
		pContext->IAGetVertexBuffers(0, 1, &veBuffer, &Stride, &veBufferOffset);
		if (veBuffer)
			veBuffer->GetDesc(&vedesc);
		if (veBuffer != NULL) { veBuffer->Release(); veBuffer = NULL; }
		else
			goto end;

		pContext->IAGetIndexBuffer(&inBuffer, &inFormat, &inOffset);
		if (inBuffer)
			inBuffer->GetDesc(&indesc);
		if (inBuffer != NULL) { inBuffer->Release(); inBuffer = NULL; }
		else
			goto end;
		//-----------------------------------模型字节函数end-------------

		if (GetAsyncKeyState(0x70) & 1)//f1键
		{	
			if (intP%2==0)
				Pcmd = 1;//关
			else
				Pcmd = 0;//开
			intP++;//2+1
			/*system("cls");
			printf("intP.2:%d\n", intP);
			printf("Pcmd.0:%d\n", Pcmd);*/
		}
		//----------测试按键----------
		if (GetAsyncKeyState('K') & 1)//-100
		{
			test_value = 100;
			g_inByteWidth -= test_value;

		}
		if (GetAsyncKeyState('I') & 1)//+100
		{
			test_value = 100;
			g_inByteWidth += test_value;

		}
		if (GetAsyncKeyState('J') & 1)//-1
		{
			test_value = 1000;
			g_inByteWidth -= test_value;
		}
		if (GetAsyncKeyState('L') & 1)//+1
		{

			test_value = 1000;
			g_inByteWidth += test_value;

		}
		//--------------按键End-------------

		//////////------------------测试输出-----------------
		///*if (Stride == 28)
		//{
		//if ((indesc.ByteWidth >= g_inByteWidth) &&
		//(indesc.ByteWidth <= g_inByteWidth + test_value))
		//{
		//system("cls");
		//printf("g_inByteWidth:%d\n", g_inByteWidth);
		//printf("Stride:%d\n", Stride);
		//printf("B:%d\n", indesc.ByteWidth);
		//return oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);
		//}

		////}
		//return;//*/
		/////////-------------------测试End--------------------------
		//
		//人物判断
		bool PlayerB((indesc.ByteWidth == 12090)	//自己下身
		//		|| (indesc.ByteWidth == 41100)	//游荡者 潜伏
		//		|| (indesc.ByteWidth == 56352)	//游荡者swat
		//		|| (indesc.ByteWidth == 59970)	//游荡者swat
				
				);
				////-----------iB:End B-28-------------
		bool PlayerT((indesc.ByteWidth == 15036)	//自己头
			|| (indesc.ByteWidth == 17580)	//萤火虫2头
			|| (indesc.ByteWidth == 17586)	//萤火虫3头
			|| (indesc.ByteWidth == 18000)	//头发1
			|| (indesc.ByteWidth == 3894)	//自己胡子
			|| (indesc.ByteWidth == 9000)	//胡子2
			|| (indesc.ByteWidth == 6000)	//全包头盔1
			|| (indesc.ByteWidth == 11988)	//全包头盔2
			|| (indesc.ByteWidth == 4482)	//肩带
			|| (indesc.ByteWidth == 16782)	//自己上身
			);
			////-----------iB:End T-28-------------

		bool PlayerTg((indesc.ByteWidth == 2706)	//自己眼珠 32
			|| (indesc.ByteWidth == 46554)	//自己头 32
			);
			//-----------iB:End T-32-------------
		//
		//if (Pcmd == 0)//人物透视
		//{
		//	if (PlayerB)
		//	{	
		//		SetDepthStencilState(DISABLED);	//禁用Z缓冲 （“显示” 被遮挡）
		//		pContext->PSSetShader(psBlue, NULL, NULL);
		//		oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);// 运行后显示这个设备上下文
		//				//		SetDepthStencilState(ENABLED);	//启用z缓冲		//		pContext->PSSetShader(psH, NULL, NULL);
		//		pContext->PSSetShaderResources(0, 1, &ShaderResourceView);//同时传入两个纹理
		//		oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);// 运行后显示这个设备上下文
		//	}
		//	
		//}
		
		//28透视 F1 0x70  大小写键0x14 鼠标侧键2 0x05 //Left Alt VK_MENU 0x12
		if (GetAsyncKeyState(0x05) & 1)
		{
			if (f1P % 2 == 0)
				f1 = false;//关
			else
				f1 = true;//开
			f1P++;//2+1
		}
		//23214瞄具 15198手枪 12090自己下身 	16782自己上身
		if (Stride == 28 && f1 &&(indesc.ByteWidth != 23214)&&(indesc.ByteWidth != 15198)&& (indesc.ByteWidth != 16782)&& (indesc.ByteWidth != 12090)) {
				SetDepthStencilState(DISABLED);	//禁用Z缓冲 （“显示” 被遮挡）
				oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);				SetDepthStencilState(ENABLED);	//启用z缓冲
		}

		//32透视 f2
		if (GetAsyncKeyState(0x71) & 1){
			if (f2P % 2 == 0)
				f2 = false;//关
			else
				f2 = true;//开
			f2P++;//2+1
		}
		if (Stride == 32 && f2) {
			SetDepthStencilState(DISABLED);	//禁用Z缓冲 （“显示” 被遮挡）
			oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);			SetDepthStencilState(ENABLED);	//启用z缓冲
		}

	end:
		return oDrawIndexed(pContext, IndexCount, StartIndexLocation, BaseVertexLocation);//重要
	}
}
