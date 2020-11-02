#pragma once

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/macros.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "CustomTypes/Components/Settings/IncrementSetting.hpp"
#include "VRUIControls/VRInputModule.hpp"
#include "modloader.hpp"
#include "Config.hpp"

DECLARE_CLASS_CODEGEN(RRQ, RRQModSettingsViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, enabledToggle);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, noteSlashDurationInc);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, noteSlashStrengthInc);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, saberClashStrengthInc);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, obstacleClashStrengthInc);
    DECLARE_INSTANCE_FIELD(QuestUI::IncrementSetting*, uiHapticStrengthInc);
    DECLARE_INSTANCE_FIELD(VRUIControls::VRInputModule*, inputModule);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool screenSystemDisabling);

    DECLARE_METHOD(void, UpdateUIComponents);
    DECLARE_METHOD(void, PreviewHapticFeedback, float strength, float duration);

    REGISTER_FUNCTION(RRQModSettingsViewController,
        REGISTER_FIELD(enabledToggle);
        REGISTER_FIELD(noteSlashDurationInc);
        REGISTER_FIELD(noteSlashStrengthInc);
        REGISTER_FIELD(saberClashStrengthInc);
        REGISTER_FIELD(obstacleClashStrengthInc);
        REGISTER_FIELD(uiHapticStrengthInc);
        REGISTER_FIELD(inputModule);

        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
        REGISTER_METHOD(UpdateUIComponents);
        REGISTER_METHOD(PreviewHapticFeedback);
    )
)

void RRQRegisterUI(ModInfo& modInfo);
