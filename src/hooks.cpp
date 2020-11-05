#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/utils.h"
#include "GlobalNamespace/NoteCutHapticEffect.hpp"
#include "GlobalNamespace/ObstacleSaberSparkleEffectManager.hpp"
#include "GlobalNamespace/SaberClashEffect.hpp"
#include "GlobalNamespace/HapticFeedbackController.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/SaberTypeExtensions.hpp"
#include "VRUIControls/VRInputModule.hpp"
#include "Libraries/HM/HMLib/VR/HapticPresetSO.hpp"
#include "rumble.hpp"
#include "rumble_private.hpp"
#include "Config.hpp"

using namespace Libraries::HM::HMLib::VR;

MAKE_HOOK_OFFSETLESS(NoteCutHapticEffect_HitNote, void, GlobalNamespace::NoteCutHapticEffect* self, GlobalNamespace::SaberType saberType) {
    static float originalStrength = self->rumblePreset->strength;
    static float originalDuration = self->rumblePreset->duration;
    static float originalFreq = self->rumblePreset->frequency;

    auto& config = getConfig();

    if (config.enabled) {
        self->rumblePreset->strength = config.noteStrength;
        self->rumblePreset->duration = config.noteDuration;
        self->rumblePreset->frequency = 1.0;
    } else {
        self->rumblePreset->strength = originalStrength;
        self->rumblePreset->duration = originalDuration;
        self->rumblePreset->frequency = originalFreq;
    }

    NoteCutHapticEffect_HitNote(self, saberType);
}

MAKE_HOOK_OFFSETLESS(ObstacleSaberSparkleEffectManager_Update, void, GlobalNamespace::ObstacleSaberSparkleEffectManager* self) {
    auto& currentConfig = getConfig();
    
    if (currentConfig.enabled) {
        self->rumblePreset->strength = currentConfig.wallStrength;
    }

    ObstacleSaberSparkleEffectManager_Update(self);
}

MAKE_HOOK_OFFSETLESS(SaberClashEffect_LateUpdate, void, GlobalNamespace::SaberClashEffect* self) {
    static float originalStrength = self->rumblePreset->strength;
    auto& currentConfig = getConfig();
    
    if (currentConfig.enabled) {
        self->rumblePreset->strength = currentConfig.saberStrength;
    } else {
        self->rumblePreset->strength = originalStrength;
    }

    SaberClashEffect_LateUpdate(self);
}

MAKE_HOOK_OFFSETLESS(VRInputModule_HandlePointerExitAndEnter, void, VRUIControls::VRInputModule* self,
                     Il2CppObject* currentPointerData, Il2CppObject* newEnterTarget)
{
    static float originalStrength = self->rumblePreset->strength;
    auto& currentConfig = getConfig();

    if (currentConfig.enabled) {
        // Block UI feedbacks if mod settings view is presenting.
        self->rumblePreset->strength = currentConfig.uiStrength; // RRQSettingsControllerIsPresenting() ? 0.0 : currentConfig.uiStrength;
    } else {
        self->rumblePreset->strength = originalStrength;
    }

    VRInputModule_HandlePointerExitAndEnter(self, currentPointerData, newEnterTarget);
}

void RRQInstallHooks() {
    getLogger().info("Adding hooks...");

    INSTALL_HOOK_OFFSETLESS(NoteCutHapticEffect_HitNote, il2cpp_utils::FindMethodUnsafe("", "NoteCutHapticEffect", "HitNote", 1));
    INSTALL_HOOK_OFFSETLESS(ObstacleSaberSparkleEffectManager_Update, il2cpp_utils::FindMethod("", "ObstacleSaberSparkleEffectManager", "Update"));
    INSTALL_HOOK_OFFSETLESS(SaberClashEffect_LateUpdate, il2cpp_utils::FindMethod("", "SaberClashEffect", "LateUpdate"));
    INSTALL_HOOK_OFFSETLESS(VRInputModule_HandlePointerExitAndEnter, il2cpp_utils::FindMethodUnsafe("VRUIControls", "VRInputModule", "HandlePointerExitAndEnter", 2));
}
