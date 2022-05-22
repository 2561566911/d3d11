#pragma once

class Helpers
{
public:
	static void HookFunction(PVOID *oFunction,PVOID pDetour);
	static void UnHookFunction(PVOID *oFunction, PVOID pDetour);
};

