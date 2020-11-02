#include "rumble_private.hpp"
#include "RRQModSettings.hpp"
#include "Config.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/XR/XRNode.hpp"
#include "GlobalNamespace/HapticFeedbackController.hpp"
#include "Libraries/HM/HMLib/VR/HapticPresetSO.hpp"
#include "QuestUI.hpp"
#include "BeatSaberUI.hpp"
#include "CustomTypes/Components/Backgroundable.hpp"

using namespace RRQ;
using namespace VRUIControls;
using namespace GlobalNamespace;
using namespace Libraries::HM::HMLib::VR;

#define BOUNDED(x, l, u) ((x) > (u) ? (u) : (x) < (l) ? (l) : (x))

DEFINE_CLASS(RRQModSettingsViewController);

static bool isPresentingModSettings = false;

// Not really using this at the moment...
bool RRQSettingsControllerIsPresenting() {
    return isPresentingModSettings;
}

void RRQModSettingsEnabledToggle(RRQModSettingsViewController* self, bool newValue) {
    auto& currentConfig = getConfig();
    getLogger().debug("enabled=%d", newValue);
    currentConfig.enabled = newValue;
}

void RRQModSettingsOnNoteDurationChange(RRQModSettingsViewController* self, float newValue) {
    auto& currentConfig = getConfig();
    float boundedValue = BOUNDED(newValue, 0.0, 60.0);

    if (boundedValue == newValue) {
        getLogger().debug("noteDuration=%f", newValue);
        if (newValue != currentConfig.noteDuration) {
            self->PreviewHapticFeedback(currentConfig.noteStrength, newValue);
            currentConfig.noteDuration = newValue;
        }
    } else {
        self->noteSlashDurationInc->CurrentValue = boundedValue;
        self->noteSlashDurationInc->UpdateValue();
    }
}

void RRQModSettingsOnNoteStrengthChange(RRQModSettingsViewController* self, float newValue) {
    auto& currentConfig = getConfig();
    float boundedValue = BOUNDED(newValue, 0.0, 1.0);

    if (boundedValue == newValue) {
        getLogger().debug("noteStrength=%f", newValue);
        if (newValue != currentConfig.noteStrength) {
            self->PreviewHapticFeedback(newValue, currentConfig.noteDuration);
            currentConfig.noteStrength = newValue;
        }
    } else {
        self->noteSlashStrengthInc->CurrentValue = boundedValue;
        self->noteSlashStrengthInc->UpdateValue();
    }
}

void RRQModSettingsOnSaberStrengthChange(RRQModSettingsViewController* self, float newValue) {
    auto& currentConfig = getConfig();
    float boundedValue = BOUNDED(newValue, 0.0, 1.0);

    if (boundedValue == newValue) {
        getLogger().debug("saberStrength=%f", newValue);
        if (currentConfig.saberStrength != newValue) {
            self->PreviewHapticFeedback(newValue, 0.3);
            currentConfig.saberStrength = newValue;
        }
    } else {
        self->saberClashStrengthInc->CurrentValue = boundedValue;
        self->saberClashStrengthInc->UpdateValue();
    }
}

void RRQModSettingsOnObstacleStrengthChange(RRQModSettingsViewController* self, float newValue) {
    auto& currentConfig = getConfig();
    float boundedValue = BOUNDED(newValue, 0.0, 1.0);

    if (boundedValue == newValue) {
        getLogger().debug("wallStrength=%f", newValue);
        if (currentConfig.wallStrength != newValue) {
            self->PreviewHapticFeedback(newValue, 0.3);
            currentConfig.wallStrength = newValue;
        }
    } else {
        self->obstacleClashStrengthInc->CurrentValue = boundedValue;
        self->obstacleClashStrengthInc->UpdateValue();
    }
}

void RRQModSettingsOnUIStrengthChange(RRQModSettingsViewController* self, float newValue) {
    auto& currentConfig = getConfig();
    float boundedValue = BOUNDED(newValue, 0.0, 1.0);

    if (boundedValue == newValue) {
        getLogger().debug("uiStrength=%f", newValue);
        if (currentConfig.uiStrength != newValue) {
            self->PreviewHapticFeedback(newValue, 0.1);
            currentConfig.uiStrength = newValue;
        }
    } else {
        self->uiHapticStrengthInc->CurrentValue = boundedValue;
        self->uiHapticStrengthInc->UpdateValue();
    }
}

void RRQModSettingsOnDefaultButtonClicked(RRQModSettingsViewController* self) {
    auto& currentConfig = getConfig();
    currentConfig.noteStrength = RRQ_DEFAULT_NOTE_STRENGTH;
    currentConfig.noteDuration = RRQ_DEFAULT_NOTE_DURATION;
    currentConfig.saberStrength = RRQ_DEFAULT_SABER_STRENGTH;
    currentConfig.wallStrength = RRQ_DEFAULT_WALL_STRENGTH;
    currentConfig.uiStrength = RRQ_DEFAULT_UI_STRENGTH;
    self->UpdateUIComponents();
}

void RRQModSettingsOnRecommendedButtonClicked(RRQModSettingsViewController* self) {
    auto& currentConfig = getConfig();
    currentConfig.noteStrength = RRQ_RECOMMENDED_NOTE_STRENGTH;
    currentConfig.noteDuration = RRQ_RECOMMENDED_NOTE_DURATION;
    currentConfig.saberStrength = RRQ_RECOMMENDED_SABER_STRENGTH;
    currentConfig.wallStrength = RRQ_RECOMMENDED_WALL_STRENGTH;
    currentConfig.uiStrength = RRQ_RECOMMENDED_UI_STRENGTH;
    self->UpdateUIComponents();
}

VRInputModule* RRQModSettingsFindInputModule() {
    auto* found = UnityEngine::Resources::FindObjectsOfTypeAll<VRInputModule*>();
    if (found->Length() > 0) {
        return found->values[0];
    } else { return nullptr; }
}

void RRQModSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation && addedToHierarchy) {
        getLogger().info("Adding mod settings UI components...");

        // Find and store VRInputModule
        inputModule = RRQModSettingsFindInputModule();

        if (inputModule) {
            getLogger().debug("Found VRInputModule.");
        }

        // Init Layout
        auto containerLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(get_rectTransform());
        containerLayout->set_spacing(1);

        auto rootLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(containerLayout->get_rectTransform());
        rootLayout->get_gameObject()->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(
            il2cpp_utils::createcsstr("round-rect-panel")
        );
        rootLayout->set_padding(UnityEngine::RectOffset::New_ctor(8, 8, 8, 8));

        auto rootLayoutElement = rootLayout->GetComponent<UnityEngine::UI::LayoutElement*>();
        rootLayoutElement->set_preferredWidth(128);
        rootLayoutElement->set_preferredHeight(128);

        auto actionsLayout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(containerLayout->get_rectTransform());
        actionsLayout->set_padding(UnityEngine::RectOffset::New_ctor(8, 8, 0, 0));
        actionsLayout->set_spacing(8);

        auto actionsLayoutElement = rootLayout->GetComponent<UnityEngine::UI::LayoutElement*>();
        actionsLayoutElement->set_preferredWidth(128);
        actionsLayoutElement->set_preferredHeight(16);

        // Create Delegates
        auto enabledToggleDelegate = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>(
            classof(UnityEngine::Events::UnityAction_1<bool>*),
            this, RRQModSettingsEnabledToggle
        );
        auto noteDurationDelegate = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*),
            this, RRQModSettingsOnNoteDurationChange
        );
        auto noteStrengthDelegate = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*),
            this, RRQModSettingsOnNoteStrengthChange
        );
        auto saberStrengthDelegate = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*),
            this, RRQModSettingsOnSaberStrengthChange
        );
        auto obstacleStrengthDelegate = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*),
            this, RRQModSettingsOnObstacleStrengthChange
        );
        auto uiStrengthDelegate = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<float>*>(
            classof(UnityEngine::Events::UnityAction_1<float>*),
            this, RRQModSettingsOnUIStrengthChange
        );
        auto defaultBtnDelegate = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(
            classof(UnityEngine::Events::UnityAction*),
            this, RRQModSettingsOnDefaultButtonClicked
        );
        auto recommendedBtnDelegate = il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(
            classof(UnityEngine::Events::UnityAction*),
            this, RRQModSettingsOnRecommendedButtonClicked
        );

        // Create UI Items
        auto& currentConfig = getConfig();

        enabledToggle = QuestUI::BeatSaberUI::CreateToggle(
            rootLayout->get_rectTransform(),
            "Enable RumbleRumbleQuest",
            currentConfig.enabled,
            enabledToggleDelegate
        );
        QuestUI::BeatSaberUI::AddHoverHint(
            enabledToggle->get_gameObject(),
            "Disable RumbleRumbleQuest to restore the default haptic behaviors. You may have to restart the game for changes to take effects."
        );

        noteSlashDurationInc = QuestUI::BeatSaberUI::CreateIncrementSetting(
            rootLayout->get_rectTransform(),
            "Note Duration",
            2, 0.01,
            currentConfig.noteDuration,
            noteDurationDelegate
        );
        QuestUI::BeatSaberUI::AddHoverHint(
            noteSlashDurationInc->get_gameObject(),
            "Adjust the duration (in seconds) of haptic feedback from slicing a block."
        );

        noteSlashStrengthInc = QuestUI::BeatSaberUI::CreateIncrementSetting(
            rootLayout->get_rectTransform(),
            "Note Strength",
            2, 0.05,
            currentConfig.noteStrength,
            noteStrengthDelegate
        );
        QuestUI::BeatSaberUI::AddHoverHint(
            noteSlashStrengthInc->get_gameObject(),
            "Adjust the strength of the haptic feedback from slicing a block."
        );

        saberClashStrengthInc = QuestUI::BeatSaberUI::CreateIncrementSetting(
            rootLayout->get_rectTransform(),
            "Saber Clash",
            2, 0.05,
            currentConfig.saberStrength,
            saberStrengthDelegate
        );
        QuestUI::BeatSaberUI::AddHoverHint(
            saberClashStrengthInc->get_gameObject(),
            "Adjust the strength of the haptic feedback from crossing the two sabers."
        );

        obstacleClashStrengthInc = QuestUI::BeatSaberUI::CreateIncrementSetting(
            rootLayout->get_rectTransform(),
            "Walls",
            2, 0.05,
            currentConfig.wallStrength,
            obstacleStrengthDelegate
        );
        QuestUI::BeatSaberUI::AddHoverHint(
            obstacleClashStrengthInc->get_gameObject(),
            "Adjust the strength of the haptic feedback from poking the sabers into the obstacles."
        );

        uiHapticStrengthInc = QuestUI::BeatSaberUI::CreateIncrementSetting(
            rootLayout->get_rectTransform(),
            "UI Haptics",
            2, 0.05,
            currentConfig.uiStrength,
            uiStrengthDelegate
        );
        QuestUI::BeatSaberUI::AddHoverHint(
            uiHapticStrengthInc->get_gameObject(),
            "Adjust the strength of the haptic feedback from interacting with UI elements."
        );

        // Actions
        QuestUI::BeatSaberUI::CreateUIButton(
            actionsLayout->get_rectTransform(),
            "Default Settings",
            "CancelButton",
            defaultBtnDelegate
        );
        QuestUI::BeatSaberUI::CreateUIButton(
            actionsLayout->get_rectTransform(),
            "Recommended Settings",
            "CancelButton",
            recommendedBtnDelegate
        );
    }
}

void RRQModSettingsViewController::DidDeactivate(bool removedFromHierarchy, bool systemScreenDisabling) {
    getLogger().info("Deactivated RRQModSettingsViewController.");
    getConfig().store();
}

void RRQModSettingsViewController::UpdateUIComponents() {
    auto& currentConfig = getConfig();
    getLogger().info("Synchronizing UI components with config...");

    noteSlashDurationInc->CurrentValue = currentConfig.noteDuration;
    noteSlashStrengthInc->CurrentValue = currentConfig.noteStrength;
    saberClashStrengthInc->CurrentValue = currentConfig.saberStrength;
    obstacleClashStrengthInc->CurrentValue = currentConfig.wallStrength;
    uiHapticStrengthInc->CurrentValue = currentConfig.uiStrength;
    
    noteSlashDurationInc->UpdateValue();
    noteSlashStrengthInc->UpdateValue();
    saberClashStrengthInc->UpdateValue();
    obstacleClashStrengthInc->UpdateValue();
    uiHapticStrengthInc->UpdateValue();
}

void RRQModSettingsViewController::PreviewHapticFeedback(float strength, float duration) {
    static HapticPresetSO* testRumblePreset = nullptr;

    getLogger().info("Playing back preview of haptic strength = %f, duration = %f", strength, duration);
    
    if (!testRumblePreset) {
        testRumblePreset = THROW_UNLESS(il2cpp_utils::New<HapticPresetSO*>());
    }

    if (inputModule) {
        // Preview params
        testRumblePreset->strength = strength;
        testRumblePreset->duration = duration;

        // Rumble on both left and right controllers
        auto leftXrNode = UnityEngine::XR::XRNode::_get_LeftHand();
        auto rightXrNode = UnityEngine::XR::XRNode::_get_RightHand();

        // Playback
        auto* hapticController = inputModule->hapticFeedbackController;
        hapticController->PlayHapticFeedback(leftXrNode, testRumblePreset);
        hapticController->PlayHapticFeedback(rightXrNode, testRumblePreset);
    } else {
        getLogger().warning("Unable to playback test haptic: undefined inputModule.");
    }
}

void RRQRegisterUI(ModInfo& modInfo) {
    getLogger().info("Registering Rumble UI...");
    QuestUI::Init();
    custom_types::Register::RegisterType<RRQModSettingsViewController>();
    QuestUI::Register::RegisterModSettingsViewController<RRQModSettingsViewController*>(modInfo, "Rumble Rumble Quest");
}
