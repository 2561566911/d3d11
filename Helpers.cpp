#include "stdafx.h"
#include "Helpers.h"

#include "./Detours-master/include/detours.h"

void Helpers::HookFunction(PVOID *oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach(oFunction, pDetour);

	DetourTransactionCommit();
}
void Helpers::UnHookFunction(PVOID *oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourDetach(oFunction, pDetour);

	DetourTransactionCommit();
}