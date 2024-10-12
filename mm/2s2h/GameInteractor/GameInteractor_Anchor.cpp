#include "GameInteractor_Anchor.h"
#include <libultraship/libultraship.h>

#include <benport.h>

extern "C" {
#include <variables.h>
#include "macros.h"
#include "z64scene.h"
#include "z64actor.h"
#include "functions.h"
#include <NameTag/NameTag.h>
extern "C" s16 gEnPartnerId;
extern PlayState* gPlayState;
extern SaveContext gSaveContext;
extern GameState* gGameState;
}

using json = nlohmann::json;

void from_json(const json& j, Color_RGB8& color) {
    j.at("r").get_to(color.r);
    j.at("g").get_to(color.g);
    j.at("b").get_to(color.b);
}

void to_json(json& j, const Color_RGB8& color) {
    j = json{ { "r", color.r }, { "g", color.g }, { "b", color.b } };
}

void to_json(json& j, const Vec3f& vec) {
    j = json{ { "x", vec.x }, { "y", vec.y }, { "z", vec.z } };
}

void to_json(json& j, const Vec3s& vec) {
    j = json{ { "x", vec.x }, { "y", vec.y }, { "z", vec.z } };
}

void from_json(const json& j, Vec3f& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
}

void from_json(const json& j, Vec3s& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
    j.at("z").get_to(vec.z);
}

void to_json(json& j, const PosRot& posRot) {
    j = json{ { "pos", posRot.pos }, { "rot", posRot.rot } };
}

void from_json(const json& j, PosRot& posRot) {
    j.at("pos").get_to(posRot.pos);
    j.at("rot").get_to(posRot.rot);
}

void from_json(const json& j, AnchorClient& client) {
    j.contains("clientId") ? j.at("clientId").get_to(client.clientId) : client.clientId = 0;
    j.contains("clientVersion") ? j.at("clientVersion").get_to(client.clientVersion) : client.clientVersion = "???";
    j.contains("name") ? j.at("name").get_to(client.name) : client.name = "???";
    j.contains("color") ? j.at("color").get_to(client.color) : client.color = { 255, 255, 255 };
    j.contains("seed") ? j.at("seed").get_to(client.seed) : client.seed = 0;
    j.contains("fileNum") ? j.at("fileNum").get_to(client.fileNum) : client.fileNum = 0xFF;
    j.contains("gameComplete") ? j.at("gameComplete").get_to(client.gameComplete) : client.gameComplete = false;
    j.contains("sceneNum") ? j.at("sceneNum").get_to(client.sceneNum) : client.sceneNum = ENTR_SCENE_MAX;
    j.contains("roomIndex") ? j.at("roomIndex").get_to(client.roomIndex) : client.roomIndex = 0;
    j.contains("entranceIndex") ? j.at("entranceIndex").get_to(client.entranceIndex) : client.entranceIndex = 0;
    j.contains("posRot") ? j.at("posRot").get_to(client.posRot) : client.posRot = { -9999, -9999, -9999, 0, 0, 0 };
}

void to_json(json& j, const PermanentSceneFlags& flags) {
    j = json{
        { "chest", flags.chest },
        { "swch0", flags.switch0 },
        { "swch1", flags.switch1 },
        { "clear", flags.clearedRoom },
        { "collectible", flags.collectible },
    };
}

void to_json(json& j, const Inventory& inventory) {
    j = json{ { "items", inventory.items },
              { "ammo", inventory.ammo },
              //{ "equipment", inventory.equipment },
              { "upgrades", inventory.upgrades },
              { "questItems", inventory.questItems },
              { "dungeonItems", inventory.dungeonItems },
              { "dungeonKeys", inventory.dungeonKeys },
              { "defenseHearts", inventory.defenseHearts },
              { "strayFairies", inventory.strayFairies } };
}

void from_json(const json& j, Inventory& inventory) {
    j.at("items").get_to(inventory.items);
    j.at("ammo").get_to(inventory.ammo);
    //j.at("equipment").get_to(inventory.equipment);
    j.at("upgrades").get_to(inventory.upgrades);
    j.at("questItems").get_to(inventory.questItems);
    j.at("dungeonItems").get_to(inventory.dungeonItems);
    j.at("dungeonKeys").get_to(inventory.dungeonKeys);
    j.at("defenseHearts").get_to(inventory.defenseHearts);
    j.at("strayFairies").get_to(inventory.strayFairies);
}

// OOT Entrance Randomizer stuff
/* void to_json(json& j, const SohStats& sohStats) {
    j = json{
        { "entrancesDiscovered", sohStats.entrancesDiscovered },
        { "fileCreatedAt", sohStats.fileCreatedAt },
    };
}

void from_json(const json& j, SohStats& sohStats) {
    j.at("entrancesDiscovered").get_to(sohStats.entrancesDiscovered);
    j.at("fileCreatedAt").get_to(sohStats.fileCreatedAt);
}*/

void to_json(json& j, const SaveContext& saveContext) {
    j = json{
        { "magicCapacity", saveContext.magicCapacity }, 
        { "masksGivenOnMoon", saveContext.masksGivenOnMoon },
        { "healthCapacity", saveContext.save.saveInfo.playerData.healthCapacity },
        { "magicLevel", saveContext.save.saveInfo.playerData.magicLevel },
        { "isMagicAcquired", saveContext.save.saveInfo.playerData.isMagicAcquired },
        { "isDoubleMagicAcquired", saveContext.save.saveInfo.playerData.isDoubleMagicAcquired },
		{ "isDoubleDefenseAcquired", saveContext.save.saveInfo.playerData.doubleDefense },
		{ "swordHealth", saveContext.save.saveInfo.playerData.swordHealth },
		// { "sceneFlags", saveContext.sceneFlags }, // ??
		{ "eventChkInf", saveContext.eventInf },
		// { "itemGetInf", saveContext.itemGetInf }, // ???
		// { "infTable", saveContext.save.saveInfo.infTable }, // ??
		// { "randomizerInf", saveContext.save.saveInfo.randomizerInf }, // No randomizer
		// { "gsFlags", saveContext.save.saveInfo.gsFlags }, // no GS
		{ "inventory", saveContext.save.saveInfo.inventory },

    };
}

void from_json(const json& j, SaveContext& saveContext) {
    j.at("magicCapacity").get_to(saveContext.magicCapacity);
    j.at("masksGivenOnMoon").get_to(saveContext.masksGivenOnMoon);
    j.at("healthCapacity").get_to(saveContext.save.saveInfo.playerData.healthCapacity);
    j.at("magicLevel").get_to(saveContext.save.saveInfo.playerData.magicLevel);
    j.at("isMagicAcquired").get_to(saveContext.save.saveInfo.playerData.isMagicAcquired);
    j.at("isDoubleMagicAcquired").get_to(saveContext.save.saveInfo.playerData.isDoubleMagicAcquired);
    j.at("isDoubleDefenseAcquired").get_to(saveContext.save.saveInfo.playerData.doubleDefense);
    j.at("swordHealth").get_to(saveContext.save.saveInfo.playerData.swordHealth);
    j.at("eventChkInf").get_to(saveContext.eventInf);
    j.at("inventory").get_to(saveContext.save.saveInfo.inventory);
}

std::map<uint32_t, AnchorClient> GameInteractorAnchor::AnchorClients = {};
std::vector<uint32_t> GameInteractorAnchor::ActorIndexToClientId = {};
// std::string GameInteractorAnchor::clientVersion = (char*)gBuildVersion; // Doesn't appear to exist in 2ship?
std::vector<std::pair<uint16_t, int16_t>> receivedItems = {};
std::vector<uint16_t> discoveredEntrances = {};
std::vector<AnchorMessage> anchorMessages = {};
uint32_t notificationId = 0;

void Anchor_DisplayMessage(AnchorMessage message = {}) {
    message.id = notificationId++;
    anchorMessages.push_back(message);
    // Audio_PlaySoundGeneral(NA_SE_SY_METRONOME, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8); // TODO: Sort out sound effect for this
}

void Anchor_SendClientData() {
    nlohmann::json payload;
    payload["data"]["name"] = CVarGetString("gRemote.AnchorName", "");
    payload["data"]["color"] = CVarGetColor24("gRemote.AnchorColor", { 100, 255, 100 });
    payload["data"]["clientVersion"] = GameInteractorAnchor::clientVersion;
    payload["type"] = "UPDATE_CLIENT_DATA";

    SPDLOG_INFO("[Anchor] A IsSaveLoaded() {}", GameInteractor::IsSaveLoaded());

    if (GameInteractor::Instance->IsSaveLoaded()) {
        // payload["data"]["seed"] = gSaveContext.finalSeed; // No randomiser, no seed.
        payload["data"]["fileNum"] = gSaveContext.fileNum;
        payload["data"]["sceneNum"] = gPlayState->sceneId;
        payload["data"]["entranceIndex"] = gSaveContext.save.entrance;
        // payload["data"]["gameComplete"] = gSaveContext.sohStats.gameComplete; // TODO find or create check for game completion
    } else {
        payload["data"]["seed"] = 0;
        payload["data"]["fileNum"] = 0xFF;
        payload["data"]["sceneNum"] = ENTR_SCENE_MAX;
        payload["data"]["entranceIndex"] = 0x00;
        payload["data"]["gameComplete"] = false;
    }

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

void GameInteractorAnchor::Enable() {
    if (isEnabled) {
        return;
    }

    isEnabled = true;
    GameInteractor::Instance->EnableRemoteInteractor();
    GameInteractor::Instance->RegisterRemoteJsonHandler([&](nlohmann::json payload) {
        try {
            HandleRemoteJson(payload);
        } catch (const std::exception& e) {
            SPDLOG_ERROR("[Anchor] Error handling remote JSON: {}", e.what());
        } catch (...) { SPDLOG_ERROR("[Anchor] Unknown Error handling remote JSON"); }
    });
    GameInteractor::Instance->RegisterRemoteConnectedHandler([&]() {
        Anchor_DisplayMessage({ .message = "Connected to Anchor" });
        Anchor_SendClientData();

        if (GameInteractor::IsSaveLoaded()) {
            Anchor_RequestSaveStateFromRemote();
        }
    });
    GameInteractor::Instance->RegisterRemoteDisconnectedHandler(
        [&]() { Anchor_DisplayMessage({ .message = "Disconnected from Anchor" }); });
}

void GameInteractorAnchor::Disable() {
    if (!isEnabled) {
        return;
    }

    isEnabled = false;
    GameInteractor::Instance->DisableRemoteInteractor();

    GameInteractorAnchor::AnchorClients.clear();
    Anchor_RefreshClientActors();
}

void GameInteractorAnchor::TransmitJsonToRemote(nlohmann::json payload) {
    payload["roomId"] = CVarGetString("gRemote.AnchorRoomId", "");
    if (!payload.contains("quiet")) {
        SPDLOG_INFO("[Anchor] Sending payload:\n{}", payload.dump());
    }
    GameInteractor::Instance->TransmitJsonToRemote(payload);
}

void Anchor_ParseSaveStateFromRemote(nlohmann::json payload);
void Anchor_PushSaveStateToRemote();

void GameInteractorAnchor::HandleRemoteJson(nlohmann::json payload) {
    if (!payload.contains("type")) {
        return;
    }

    if (!payload.contains("quiet")) {
        SPDLOG_INFO("[Anchor] Received payload:\n{}", payload.dump());
    }

    if ((payload["type"] != "ALL_CLIENT_DATA" && payload["type"] != "UPDATE_CLIENT_DATA")) {
        if (payload.contains("clientId")) {
            uint32_t clientId = payload["clientId"].get<uint32_t>();
            if (GameInteractorAnchor::AnchorClients.contains(clientId) &&
                GameInteractorAnchor::AnchorClients[clientId].clientVersion != GameInteractorAnchor::clientVersion) {
                return;
            }
        }
    }

    // --- \\

    if (payload["type"] == "GIVE_ITEM") {
        /*
        // TODO GameInteractionEffect
        auto effect = new GameInteractionEffect::GiveItem();
        effect->parameters[0] = payload["modId"].get<uint16_t>();
        effect->parameters[1] = payload["getItemId"].get<int16_t>();
        CVarSetInteger("gFromGI", 1);
        receivedItems.push_back({ payload["modId"].get<uint16_t>(), payload["getItemId"].get<int16_t>() });

        // TODO work out what "Possible" is, probably related to GameInteractionEffect
        if (effect->Apply() == Possible) {
            GetItemEntry getItemEntry =
                ItemTableManager::Instance->RetrieveItemEntry(effect->parameters[0], effect->parameters[1]);

            AnchorClient anchorClient = GameInteractorAnchor::AnchorClients[payload["clientId"].get<uint32_t>()];
            if (getItemEntry.getItemCategory != ITEM_CATEGORY_JUNK) {
                if (getItemEntry.modIndex == MOD_NONE) {
                    Anchor_DisplayMessage({ .itemIcon = SohUtils::GetIconNameFromItemID(getItemEntry.itemId),
                                            .prefix = SohUtils::GetItemName(getItemEntry.itemId),
                                            .message = "from",
                                            .suffix = anchorClient.name });
                } else if (getItemEntry.modIndex == MOD_RANDOMIZER) {
                    Anchor_DisplayMessage(
                        { .itemIcon = SohUtils::GetIconNameFromItemID(
                              SohUtils::GetItemIdIconFromRandomizerGet(getItemEntry.getItemId)),
                          .prefix = OTRGlobals::Instance->gRandomizer
                                        ->EnumToSpoilerfileGetName[(RandomizerGet)getItemEntry.getItemId]
                                                                  [gSaveContext.language],
                          .message = "from",
                          .suffix = anchorClient.name });
                }
            }
        }
        CVarClear("gFromGI");
        */
    }

    // --- \\

    if (payload["type"] == "SET_SCENE_FLAG") {
        /* auto effect = new GameInteractionEffect::SetSceneFlag();
        effect->parameters[0] = payload["sceneNum"].get<int16_t>();
        effect->parameters[1] = payload["flagType"].get<int16_t>();
        effect->parameters[2] = payload["flag"].get<int16_t>();
        effect->Apply();*/ // TODO GameInteractionEffect
    }
    if (payload["type"] == "SET_FLAG") {
        /* auto effect = new GameInteractionEffect::SetFlag();
        effect->parameters[0] = payload["flagType"].get<int16_t>();
        effect->parameters[1] = payload["flag"].get<int16_t>();
        effect->Apply();*/ // TODO GameInteractionEffect

        // If mweep flag replace ruto's letter
        /* if (payload["flagType"].get<int16_t>() == FLAG_EVENT_CHECK_INF &&
            payload["flag"].get<int16_t>() == EVENTCHKINF_KING_ZORA_MOVED &&
            Inventory_HasSpecificBottle(ITEM_LETTER_RUTO)) {
            Inventory_ReplaceItem(gPlayState, ITEM_LETTER_RUTO, ITEM_BOTTLE);
        }*/ // Mweep king doesn't exist in MM.
    }
    if (payload["type"] == "UNSET_SCENE_FLAG") {
        /* auto effect = new GameInteractionEffect::UnsetSceneFlag();
        effect->parameters[0] = payload["sceneNum"].get<int16_t>();
        effect->parameters[1] = payload["flagType"].get<int16_t>();
        effect->parameters[2] = payload["flag"].get<int16_t>();
        effect->Apply();*/ // TODO: GameInteractionEffect
    }
    if (payload["type"] == "UNSET_FLAG") {
        /* auto effect = new GameInteractionEffect::UnsetFlag();
        effect->parameters[0] = payload["flagType"].get<int16_t>();
        effect->parameters[1] = payload["flag"].get<int16_t>();
        effect->Apply();*/ // TODO: GameInteractionEffect
    }
    if (payload["type"] == "CLIENT_UPDATE") {
        uint32_t clientId = payload["clientId"].get<uint32_t>();

        if (GameInteractorAnchor::AnchorClients.contains(clientId)) {
            GameInteractorAnchor::AnchorClients[clientId].sceneNum = payload["sceneNum"].get<int16_t>();
            GameInteractorAnchor::AnchorClients[clientId].roomIndex =
                payload.contains("roomIndex") ? payload.at("roomIndex").get<int16_t>() : 0;
            GameInteractorAnchor::AnchorClients[clientId].entranceIndex =
                payload.contains("entranceIndex") ? payload.at("entranceIndex").get<int16_t>() : 0;
            GameInteractorAnchor::AnchorClients[clientId].posRot = payload["posRot"].get<PosRot>();
        }
    }
    if (payload["type"] == "PUSH_SAVE_STATE" && GameInteractor::IsSaveLoaded()) {
        Anchor_ParseSaveStateFromRemote(payload);
    }
    if (payload["type"] == "REQUEST_SAVE_STATE" && GameInteractor::IsSaveLoaded()) {
        Anchor_PushSaveStateToRemote();
    }

    // --- \\

    if (payload["type"] == "ALL_CLIENT_DATA") {
        std::vector<AnchorClient> newClients = payload["clients"].get<std::vector<AnchorClient>>();

        // add new clients
        for (auto& client : newClients) {
            if (!GameInteractorAnchor::AnchorClients.contains(client.clientId)) {
                GameInteractorAnchor::AnchorClients[client.clientId] = { client.clientId,
                                                                         client.clientVersion,
                                                                         client.name,
                                                                         client.color,
                                                                         client.seed,
                                                                         client.fileNum,
                                                                         client.gameComplete,
                                                                         client.sceneNum,
                                                                         0,
                                                                         client.entranceIndex,
                                                                         { -9999, -9999, -9999, 0, 0, 0 } };
                Anchor_DisplayMessage(
                    { .prefix = client.name, .prefixColor = ImVec4(1.0f, 0.5f, 0.5f, 1.0f), .message = "connected" });
            }
        }

        // remove clients that are no longer in the list
        std::vector<uint32_t> clientsToRemove;
        for (auto& [clientId, client] : GameInteractorAnchor::AnchorClients) {
            if (std::find_if(newClients.begin(), newClients.end(), [clientId = clientId](AnchorClient& c) {
                    return c.clientId == clientId;
                }) == newClients.end()) {
                clientsToRemove.push_back(clientId);
            }
        }
        for (auto& clientId : clientsToRemove) {
            Anchor_DisplayMessage({ .prefix = GameInteractorAnchor::AnchorClients[clientId].name,
                                    .prefixColor = ImVec4(1.0f, 0.5f, 0.5f, 1.0f),
                                    .message = "disconnected" });
            GameInteractorAnchor::AnchorClients.erase(clientId);
        }

        Anchor_RefreshClientActors();
    }

    // --- \\

    if (payload["type"] == "UPDATE_CLIENT_DATA") {
        uint32_t clientId = payload["clientId"].get<uint32_t>();
        if (GameInteractorAnchor::AnchorClients.contains(clientId)) {
            AnchorClient client = payload["data"].get<AnchorClient>();
            GameInteractorAnchor::AnchorClients[clientId].clientVersion = client.clientVersion;
            GameInteractorAnchor::AnchorClients[clientId].name = client.name;
            GameInteractorAnchor::AnchorClients[clientId].color = client.color;
            GameInteractorAnchor::AnchorClients[clientId].seed = client.seed;
            GameInteractorAnchor::AnchorClients[clientId].fileNum = client.fileNum;
            GameInteractorAnchor::AnchorClients[clientId].gameComplete = client.gameComplete;
            GameInteractorAnchor::AnchorClients[clientId].sceneNum = client.sceneNum;
            GameInteractorAnchor::AnchorClients[clientId].entranceIndex = client.entranceIndex;
        }
    }
    if (payload["type"] == "UPDATE_CHECK_DATA" && GameInteractor::IsSaveLoaded()) {
        /* auto check = payload["locationIndex"].get<uint32_t>();
        auto data = payload["checkData"].get<RandomizerCheckTrackerData>();
        CheckTracker::UpdateCheck(check, data);*/ // Relates to OOT randomizer change tracker
    }
    if (payload["type"] == "ENTRANCE_DISCOVERED") {
        /* auto entranceIndex = payload["entranceIndex"].get<uint16_t>();
        discoveredEntrances.push_back(entranceIndex);
        Entrance_SetEntranceDiscovered(entranceIndex, 1);*/ // Seems to be directly related to OOT entrance randomizer
    }
    if (payload["type"] == "UPDATE_BEANS_BOUGHT" && GameInteractor::IsSaveLoaded()) {
        BEANS_BOUGHT = payload["amount"].get<uint8_t>();
    }
    if (payload["type"] == "UPDATE_BEANS_COUNT" && GameInteractor::IsSaveLoaded()) {
        AMMO(ITEM_MAGIC_BEANS) = payload["amount"].get<uint8_t>();
    }
    if (payload["type"] == "CONSUME_ADULT_TRADE_ITEM" && GameInteractor::IsSaveLoaded()) {
        /* uint8_t itemId = payload["itemId"].get<uint8_t>();
        gSaveContext.adultTradeItems &= ~ADULT_TRADE_FLAG(itemId);
        Inventory_ReplaceItem(gPlayState, itemId, Randomizer_GetNextAdultTradeItem());*/ // No adult trade in MM
    }
    if (payload["type"] == "UPDATE_KEY_COUNT" && GameInteractor::IsSaveLoaded()) {
        gSaveContext.save.saveInfo.inventory.dungeonKeys[payload["sceneNum"].get<int16_t>()] =
            payload["amount"].get<int8_t>();
    }
    if (payload["type"] == "GIVE_DUNGEON_ITEM" && GameInteractor::IsSaveLoaded()) {
        gSaveContext.save.saveInfo.inventory.dungeonItems[payload["sceneNum"].get<int16_t>()] |=
            gBitFlags[payload["itemId"].get<uint16_t>() - ITEM_KEY_BOSS];
    }
    if (payload["type"] == "GAME_COMPLETE") {
        AnchorClient anchorClient = GameInteractorAnchor::AnchorClients[payload["clientId"].get<uint32_t>()];
        Anchor_DisplayMessage({
            .prefix = anchorClient.name,
            .message = "has defeated Majora.",
        });
    }
    if (payload["type"] == "REQUEST_TELEPORT") {
        Anchor_TeleportToPlayer(payload["clientId"].get<uint32_t>());
    }
    if (payload["type"] == "TELEPORT_TO") {
        uint32_t entranceIndex = payload["entranceIndex"].get<uint32_t>();
        uint32_t roomIndex = payload["roomIndex"].get<uint32_t>();
        PosRot posRot = payload["posRot"].get<PosRot>();

        Play_SetRespawnData(gGameState, RESPAWN_MODE_DOWN, entranceIndex, roomIndex, 0xDFF, &posRot.pos, posRot.rot.y);
        // Play_TriggerVoidOut(gPlayState); // TODO: voidout?
    }
    if (payload["type"] == "SERVER_MESSAGE") {
        Anchor_DisplayMessage({
            .prefix = "Server:",
            .prefixColor = ImVec4(1.0f, 0.5f, 0.5f, 1.0f),
            .message = payload["message"].get<std::string>(),
        });
    }
    if (payload["type"] == "DISABLE_ANCHOR") {
        GameInteractor::Instance->isRemoteInteractorEnabled = false;
        GameInteractorAnchor::Instance->isEnabled = false;
    }
    if (payload["type"] == "RESET") {
        std::reinterpret_pointer_cast<Ship::ConsoleWindow>(
            Ship::Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Console"))
            ->Dispatch("reset");
    }
}

void Anchor_PushSaveStateToRemote() {
    json payload = gSaveContext;
    payload["type"] = "PUSH_SAVE_STATE";

    // manually update current scene flags
    payload["sceneFlags"][gPlayState->sceneId]["chest"] = gPlayState->actorCtx.flags.chest;
    payload["sceneFlags"][gPlayState->sceneId]["swch"] = gPlayState->actorCtx.flags.swch;
    //payload["sceneFlags"][gPlayState->sceneId]["swch1"] = gPlayState->actorCtx.flags.swch1;
    payload["sceneFlags"][gPlayState->sceneId]["clear"] = gPlayState->actorCtx.flags.clear;
    payload["sceneFlags"][gPlayState->sceneId]["collectible"] = gPlayState->actorCtx.flags.collect;

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

void Anchor_RequestSaveStateFromRemote() {
    nlohmann::json payload;
    payload["type"] = "REQUEST_SAVE_STATE";

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

void Anchor_ParseSaveStateFromRemote(nlohmann::json payload) {
    SaveContext loadedData = payload.get<SaveContext>();

    gSaveContext.magicCapacity = gSaveContext.save.saveInfo.playerData.magic = loadedData.magicCapacity;
    // gSaveContext.masksGivenOnMoon = loadedData.masksGivenOnMoon; // Can't modify? TODO: Check
    gSaveContext.save.saveInfo.playerData.healthCapacity = loadedData.save.saveInfo.playerData.healthCapacity;
    gSaveContext.save.saveInfo.playerData.magicLevel = loadedData.save.saveInfo.playerData.magicLevel;
    gSaveContext.save.saveInfo.playerData.isMagicAcquired = loadedData.save.saveInfo.playerData.isMagicAcquired;
    gSaveContext.save.saveInfo.playerData.isDoubleMagicAcquired =
        loadedData.save.saveInfo.playerData.isDoubleMagicAcquired;
    gSaveContext.save.saveInfo.playerData.doubleDefense = loadedData.save.saveInfo.playerData.doubleDefense;
    gSaveContext.save.saveInfo.playerData.swordHealth = loadedData.save.saveInfo.playerData.swordHealth;
    // gSaveContext.eventInf = loadedData.eventInf; // Can't modify? TODO: Check
    gSaveContext.save.saveInfo.inventory = loadedData.save.saveInfo.inventory;

}

AnchorClient* Anchor_GetClientByActorIndex(uint32_t actorIndex) {
    if (actorIndex < GameInteractorAnchor::ActorIndexToClientId.size()) {
        uint32_t clientId = GameInteractorAnchor::ActorIndexToClientId[actorIndex];
        if (GameInteractorAnchor::AnchorClients.find(clientId) != GameInteractorAnchor::AnchorClients.end()) {
            return &GameInteractorAnchor::AnchorClients[clientId];
        }
    }

    return nullptr;
}

uint8_t Anchor_GetClientScene(uint32_t actorIndex) {
    AnchorClient* client = Anchor_GetClientByActorIndex(actorIndex);
    if (client == nullptr)
        return ENTR_SCENE_MAX;

    return client->sceneNum;
}

PosRot Anchor_GetClientPosition(uint32_t actorIndex) {
    AnchorClient* client = Anchor_GetClientByActorIndex(actorIndex);
    if (client == nullptr)
        return { -9999.0, -9999.0, -9999.0, 0, 0, 0 };

    return client->posRot;
}

uint8_t Anchor_GetClientRoomIndex(uint32_t actorIndex) {
    AnchorClient* client = Anchor_GetClientByActorIndex(actorIndex);
    if (client == nullptr)
        return 0xFF;

    return client->roomIndex;
}

Color_RGB8 Anchor_GetClientColor(uint32_t actorIndex) {
    AnchorClient* client = Anchor_GetClientByActorIndex(actorIndex);
    if (client == nullptr)
        return { 100, 255, 100 };

    return client->color;
}

void Anchor_RefreshClientActors() {
    if (!GameInteractor::IsSaveLoaded())
        return;
    Actor* actor = NULL; gPlayState->actorCtx.actorLists[ACTORCAT_ITEMACTION].first; // Assuming 'first' was the same as 'head' in SoH
    while (actor != NULL) {
        if (gEnPartnerId == actor->id) {
            Actor_Kill(actor);
        }
        actor = actor->next;
    }

    GameInteractorAnchor::ActorIndexToClientId.clear();

    uint32_t i = 0;
    for (auto [clientId, client] : GameInteractorAnchor::AnchorClients) {
        GameInteractorAnchor::ActorIndexToClientId.push_back(clientId);
        auto fairy = Actor_Spawn(&gPlayState->actorCtx, gPlayState, gEnPartnerId, client.posRot.pos.x,
                                 client.posRot.pos.y, client.posRot.pos.z, client.posRot.rot.x, client.posRot.rot.y, client.posRot.rot.z, 3 + i);//, false); // Last param seems to be for randomiser

        NameTag_RegisterForActor(fairy, client.name.c_str());
        i++;
    }
}

static uint32_t lastSceneNum = ENTR_SCENE_MAX;

void Anchor_RegisterHooks() {

}

void Anchor_EntranceDiscovered(uint16_t entranceIndex) {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;

    // If the entrance exists in discoveredEntrances, remove it from the list and don't emit the packet
    auto it = std::find(discoveredEntrances.begin(), discoveredEntrances.end(), entranceIndex);
    if (it != discoveredEntrances.end()) {
        discoveredEntrances.erase(it);
        return;
    }

    nlohmann::json payload;

    payload["type"] = "ENTRANCE_DISCOVERED";
    payload["entranceIndex"] = entranceIndex;

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

// I think this is randomiser item tracker stuff
/* void Anchor_UpdateCheckData(uint32_t locationIndex) {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;

    nlohmann::json payload;

    payload["type"] = "UPDATE_CHECK_DATA";
    payload["locationIndex"] = locationIndex;
    payload["checkData"] = gSaveContext.checkTrackerData[locationIndex];
    if (gSaveContext.checkTrackerData[locationIndex].status == RCSHOW_COLLECTED) {
        payload["checkData"]["status"] = RCSHOW_SAVED;
    }

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}*/

void Anchor_UpdateBeansBought(uint8_t amount) {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;

    nlohmann::json payload;

    payload["type"] = "UPDATE_BEANS_BOUGHT";
    payload["amount"] = amount;

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

void Anchor_UpdateBeansCount(uint8_t amount) {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;

    nlohmann::json payload;

    payload["type"] = "UPDATE_BEANS_COUNT";
    payload["amount"] = amount;

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

void Anchor_ConsumeAdultTradeItem(uint8_t itemId) {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;

    nlohmann::json payload;

    payload["type"] = "CONSUME_ADULT_TRADE_ITEM";
    payload["itemId"] = itemId;

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

void Anchor_UpdateKeyCount(int16_t sceneNum, int8_t amount) {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;

    nlohmann::json payload;

    payload["type"] = "UPDATE_KEY_COUNT";
    payload["sceneNum"] = sceneNum;
    payload["amount"] = amount;

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

void Anchor_GiveDungeonItem(int16_t sceneNum, uint16_t itemId) {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;

    nlohmann::json payload;

    payload["type"] = "GIVE_DUNGEON_ITEM";
    payload["sceneNum"] = sceneNum;
    payload["itemId"] = itemId;

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

void Anchor_GameComplete() {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;

    nlohmann::json payload;

    payload["type"] = "GAME_COMPLETE";

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
    Anchor_SendClientData();
}

void Anchor_RequestTeleport(uint32_t clientId) {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;

    nlohmann::json payload;

    payload["type"] = "REQUEST_TELEPORT";
    payload["targetClientId"] = clientId;

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

void Anchor_TeleportToPlayer(uint32_t clientId) {
    if (!GameInteractor::Instance->isRemoteInteractorConnected || !GameInteractor::Instance->IsSaveLoaded())
        return;
    Player* player = GET_PLAYER(gPlayState);

    nlohmann::json payload;

    payload["type"] = "TELEPORT_TO";
    payload["targetClientId"] = clientId;
    payload["entranceIndex"] = gSaveContext.save.entrance; //.entranceIndex;
    payload["roomIndex"] = gPlayState->roomCtx.curRoom.num;
    payload["posRot"] = player->actor.world;

    GameInteractorAnchor::Instance->TransmitJsonToRemote(payload);
}

// TODO Player location UI
const ImVec4 GRAY = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
const ImVec4 WHITE = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
const ImVec4 GREEN = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);

void AnchorPlayerLocationWindow::DrawElement() {
}

void AnchorLogWindow::DrawElement() {
}

void AnchorLogWindow::UpdateElement() {
}