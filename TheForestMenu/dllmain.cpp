﻿#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>

#include "haxsdk/haxsdk.h"
#include "cheat.h"

static void Start() {
	LOG_INIT(DEBUG, true);
	HaxSdk::InitializeBackendData();
    HaxSdk::InitializeUnityData();
    HaxSdk::InitializeGameData();
	HaxSdk::ImplementImGui(GraphicsApi_DirectX11);
}

bool __stdcall DllMain(HMODULE module, DWORD reason, LPVOID lpvReserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Start, NULL, 0, NULL);
	}
	return true;
}