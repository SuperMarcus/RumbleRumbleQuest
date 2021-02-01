#include "modloader.hpp"
#include "rumble_private.hpp"
#include "rumble.hpp"
#include "Config.hpp"
#include "RRQModSettings.hpp"

static ModInfo modInfo;

Logger& getLogger() {
    static auto* logger = new Logger(modInfo);
    return *logger;
}

ConfigClass& getConfig() {
    static Configuration raw(modInfo);
    static ConfigClass config(raw);
    return config;
}

extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
    getLogger().info("Setting up Rumble mod.");
}

extern "C" void load() {
    getLogger().info("Loading Rumble...");
    RRQInstallHooks();
    RRQRegisterUI(modInfo);
    getLogger().info("Finished loading Rumble.");
}
