#include "Config.hpp"
#include "rumble_private.hpp"

#define SET_PRIMITIVE(doc, name, type, value, alloc) \
    if (doc .HasMember(name)) { \
        doc [name].Set ## type (value); \
    } else { \
        doc .AddMember(name, value, alloc); \
    }

#define GET_PRIMITIVE(doc, name, type, default) \
    doc .HasMember(name) && doc[name].Is ## type () \
    ? doc [name].Get ## type () : (default)

ConfigClass::ConfigClass(Configuration& storage): _storage(storage) {
    load();
}

void ConfigClass::load() {
    getLogger().info("Loading configurations...");

    _storage.Load();
    auto& document = _storage.config;
    
    enabled = GET_PRIMITIVE(document, "enabled", Bool, true);
    noteStrength = GET_PRIMITIVE(document, "note_strength", Float, RRQ_DEFAULT_NOTE_STRENGTH);
    noteDuration = GET_PRIMITIVE(document, "note_duration", Float, RRQ_DEFAULT_NOTE_DURATION);
    saberStrength = GET_PRIMITIVE(document, "saber_strength", Float, RRQ_DEFAULT_SABER_STRENGTH);
    wallStrength = GET_PRIMITIVE(document, "wall_strength", Float, RRQ_DEFAULT_WALL_STRENGTH);
    uiStrength = GET_PRIMITIVE(document, "ui_strength", Float, RRQ_DEFAULT_UI_STRENGTH);
}

void ConfigClass::store() {
    auto& document = _storage.config;
    auto& allocator = document.GetAllocator();

    SET_PRIMITIVE(document, "enabled", Bool, enabled, allocator);
    SET_PRIMITIVE(document, "note_strength", Float, noteStrength, allocator);
    SET_PRIMITIVE(document, "note_duration", Float, noteDuration, allocator);
    SET_PRIMITIVE(document, "saber_strength", Float, saberStrength, allocator);
    SET_PRIMITIVE(document, "wall_strength", Bool, wallStrength, allocator);
    SET_PRIMITIVE(document, "ui_strength", Bool, uiStrength, allocator);

    _storage.Write();
    getLogger().info("Configuration updated.");
}
