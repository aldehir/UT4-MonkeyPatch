#include "Core.h"
#include "Engine.h"
#include "ModuleManager.h"
#include "ModuleInterface.h"

#include "Windows/WindowsHWrapper.h"
#include "funchook.h"

#include "Symbols.h"

class FMonkeyPatch : public IModuleInterface
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    funchook_t* Hooks;
    void* EngineHandle;
};

IMPLEMENT_MODULE(FMonkeyPatch, MonkeyPatch)

DEFINE_LOG_CATEGORY_STATIC(LogMonkeyPatch, Log, All)

typedef void* (*GetCookedDataFunc)(void* t);

GetCookedDataFunc UNavCollision_GetCookedData;

static void* GetCookedDataHook(void* t);


void FMonkeyPatch::StartupModule()
{
    Hooks = funchook_create();

    FModuleManager& man = FModuleManager::Get();

    FName engineModName(TEXT("Engine"));

    if (!man.IsModuleLoaded(engineModName) && !man.LoadModule(engineModName).IsValid())
    {
        UE_LOG(LogMonkeyPatch, Log, TEXT("Could not load module: %s"), *engineModName.ToString());
        return;
    }

    FString engineModFile = man.GetModuleFilename(engineModName);

    UE_LOG(LogMonkeyPatch, Log, TEXT("Engine module found: %s"), *engineModFile);

    void* handle = FPlatformProcess::GetDllHandle(*engineModFile);
    if (handle == nullptr)
    {
        UE_LOG(LogMonkeyPatch, Log, TEXT("Could not get DLL handle for %s"), *engineModName.ToString());
        return;
    }

    EngineHandle = handle;

    void* proc = FPlatformProcess::GetDllExport(EngineHandle, TEXT(SYMBOL_UNAVCOLLISION_GETHOOKDATA));
    if (proc == nullptr) {
        UE_LOG(LogMonkeyPatch, Log, TEXT("Could not find UNavCollision::GetCookedData"));
        return;
    }

    UE_LOG(LogMonkeyPatch, Log, TEXT("Found UNavCollision::GetCookedData @ %lx"), proc);

    UNavCollision_GetCookedData = (GetCookedDataFunc)proc;

    if (funchook_prepare(Hooks, (void**)&UNavCollision_GetCookedData, GetCookedDataHook) != 0) {
        UE_LOG(LogMonkeyPatch, Log, TEXT("Failed to prepare function hook."));
        return;
    }

    if (funchook_install(Hooks, 0) != 0) {
        UE_LOG(LogMonkeyPatch, Log, TEXT("Failed to install function hooks."));
        return;
    }

    UE_LOG(LogMonkeyPatch, Log, TEXT("Monkey Patch complete."));
}

void FMonkeyPatch::ShutdownModule()
{
    if (Hooks != nullptr)
    {
        funchook_uninstall(Hooks, 0);
        funchook_destroy(Hooks);
        Hooks = nullptr;
    }

    if (EngineHandle != nullptr)
    {
        FPlatformProcess::FreeDllHandle(EngineHandle);
        EngineHandle = nullptr;
    }
}

static void* GetCookedDataHook(void* t) {
    return NULL;
}
