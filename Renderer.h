#pragma once
#pragma once
#include "stdafx.h"
#undef DrawText

class  Renderer
{
public:
	void Initialize();

	static Renderer* GetInstance();

private:
	static Renderer* m_pInstance;
};