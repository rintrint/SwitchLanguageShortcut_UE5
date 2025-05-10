#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSwitchLanguageShortcutModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void RegisterMenus();
    void SwitchLanguage();
    TSharedPtr<class FUICommandList> PluginCommands;
};