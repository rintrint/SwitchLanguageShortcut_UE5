#include "SwitchLanguageShortcut.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandList.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "FSwitchLanguageShortcutModule"

class FSwitchLanguageShortcutCommands : public TCommands<FSwitchLanguageShortcutCommands>
{
public:
    FSwitchLanguageShortcutCommands()
        : TCommands<FSwitchLanguageShortcutCommands>(
            TEXT("SwitchLanguageShortcut"),
            NSLOCTEXT("Contexts", "SwitchLanguageShortcut", "SwitchLanguageShortcut Plugin"),
            NAME_None,
            FAppStyle::GetAppStyleSetName())
    {}

    virtual void RegisterCommands() override
    {
        UI_COMMAND(PluginAction, "Switch Language", "Switch between Chinese and English", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Alt, EKeys::T));
    }

public:
    TSharedPtr<FUICommandInfo> PluginAction;
};

void FSwitchLanguageShortcutModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("SwitchLanguageShortcut module starting up"));
    FSwitchLanguageShortcutCommands::Register();
    UE_LOG(LogTemp, Log, TEXT("SwitchLanguageShortcut commands registered"));
    
    PluginCommands = MakeShareable(new FUICommandList);
    PluginCommands->MapAction(
        FSwitchLanguageShortcutCommands::Get().PluginAction,
        FExecuteAction::CreateRaw(this, &FSwitchLanguageShortcutModule::SwitchLanguage),
        FCanExecuteAction());
    UE_LOG(LogTemp, Log, TEXT("SwitchLanguageShortcut shortcut mapped"));

    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSwitchLanguageShortcutModule::RegisterMenus));
    
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
        "SwitchLanguageShortcut",
        FOnSpawnTab::CreateLambda([](const FSpawnTabArgs&) { return SNew(SDockTab); })
    )
    .SetDisplayName(LOCTEXT("SwitchLanguageShortcutTab", "Switch Language"))
    .SetMenuType(ETabSpawnerMenuType::Hidden);

    // Register the commands with the global command list
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    LevelEditorModule.GetGlobalLevelEditorActions()->Append(PluginCommands.ToSharedRef());
}

void FSwitchLanguageShortcutModule::ShutdownModule()
{
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);
    FSwitchLanguageShortcutCommands::Unregister();
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("SwitchLanguageShortcut");
}

void FSwitchLanguageShortcutModule::RegisterMenus()
{
    UE_LOG(LogTemp, Log, TEXT("SwitchLanguageShortcut registering menus"));
    FToolMenuOwnerScoped OwnerScoped(this);
    
    UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
    {
        FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
        {
            Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSwitchLanguageShortcutCommands::Get().PluginAction));
        }
    }
}

void FSwitchLanguageShortcutModule::SwitchLanguage()
{
    UE_LOG(LogTemp, Log, TEXT("SwitchLanguage called"));
    
    FString CurrentCulture = FInternationalization::Get().GetCurrentCulture()->GetName();
    FString NewCulture = CurrentCulture == TEXT("en") ? TEXT("zh-CN") : TEXT("en");
    
    UE_LOG(LogTemp, Log, TEXT("Switching language from %s to %s"), *CurrentCulture, *NewCulture);
    
    FInternationalization::Get().SetCurrentCulture(NewCulture);
    
    // Attempt to refresh the UI (this might not work perfectly in all cases)
    if (FSlateApplication::IsInitialized())
    {
        FSlateApplication::Get().InvalidateAllWidgets(true);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Language switch completed"));
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSwitchLanguageShortcutModule, SwitchLanguageShortcut)