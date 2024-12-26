#include "cheat.h"

#if !defined(PATRON) && !defined(FREE)
#error Either PATRON or FREE must be specified
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>
#include <vector>
#include <format>
#include <algorithm>

#include "haxsdk/haxsdk.h"
#include "haxsdk/third_party/imgui/imgui.h"
#include "haxsdk/third_party/detours/x64/detours.h"
#include "font.h"
#include "locales.h"
#include "image.h"

struct mutantController;
struct Scene;
struct Cheats;
struct DebugConsole;
struct AchievementData;
struct WeatherSystem;
struct SpawnPoolsDict;
struct FirstPersonCharacter;
struct PlayerStats;
struct FuelData;
struct InventoryItem;
struct LocalPlayer;
struct sceneTracker;
struct Item;
struct ItemDatabase;
struct PlayerInventory;
struct TreeHealth;
struct animalHealth;
struct EnemyHealth;

#define HAXSDK_FUNCTION(a, n, c, m, s)          static Method c ## __ ## m
#define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  static Method c ## __ ## m
#define HAXSDK_STATIC_FIELD(a, n, c, f, t)      static t* c ## __ ## f
#define HAXSDK_FIELD_OFFSET(a, n, c, f)         static int c ## __ ## f
#include "cheat_data.h"

static ImTextureID g_BoostyTexture = nullptr;
static ImVec2 g_textureSize;
static ImVec2 g_modMenuSize = ImVec2(445.f, 600.f);

static int g_curLocale = LOCALE_ENG;
static ImFont* g_espFont;
static float g_sizeScale = 1;
static bool g_inGame = false;
static bool g_infiniteBattery = false;
static bool g_maxFullness = false;
static bool g_noThirst = false;
static bool g_buildHack = false;
static bool g_unlimitedItems = false;
static bool g_noFallDamage = false;
static float g_speedMult = 1.F;
static float g_jumpMult = 1.F;
static bool g_invisible = false;
static bool g_noCold = false;
static bool g_maxFuel = false;
static bool g_animalEsp= false;
static bool g_cannibalEsp = false;
static bool g_locationsEsp = false;
static bool g_ohk = false;
static bool g_doLight = false;
static bool g_infConsumables = false;
static bool g_fastTreeCut = false;
static DebugConsole* g_debugConsole = nullptr;
static Unity::GameObject* g_light = nullptr;
static bool g_speedUpGame = false;
static bool g_skipPlaneCrash = false;

static bool g_forceRain = false;
static int g_forceRainType = 0;

static bool g_forceAllItems = false;
static bool g_forceStoryItems = false;
static bool g_forceItem = false;
static int32_t g_forceItemId = 0;
static bool g_forceRandomOutfit = false;

static int g_curEnemy = 0;
static bool g_forceEnemySpawn = false;
static int g_curAnimal = 0;
static bool g_forceAnimalSpawn = false;

static bool g_forceAchievements = false;

static int g_curArmor = 0;
static bool g_forceArmor = false;

static const char* enemiesInternalNames[13] = {
    "male_skinny",
    "female_skinny",
    "skinny_pale",
    "male",
    "female",
    "fireman",
    "dynamiteman",
    "pale",
    "armsy",
    "vags",
    "baby",
    "fat",
    "girl"
};

static const char* animalsInternalNames[8] = {
    "rabbitGo",
    "lizardGo",
    "deerGo",
    "tortoiseGo",
    "raccoonGo",
    "squirrelGo",
    "boarGo",
    "crocodileGo"
};

static void Hooked_LocalPlayer__Awake(LocalPlayer* __this);
static void Hooked_LocalPlayer__OnDestroy(LocalPlayer* __this);
static void Hooked_PlayerStats__Update(PlayerStats* __this);
static bool Hooked_PlayerStats__get_Warm(PlayerStats* __this);
static void Hooked_BatteryBasedLight__Update(void* __this);
static int  Hooked_InventoryItem__Add(InventoryItem* __this, int32_t amount, bool isEquiped);
static void Hooked_FirstPersonCharacter__HandleLanded(FirstPersonCharacter* __this);
static void Hooked_FirstPersonCharacter__HandleWalkingSpeedOptions(FirstPersonCharacter* __this);
static void Hooked_FirstPersonCharacter__HandleRunningStaminaAndSpeed(FirstPersonCharacter* __this);
static float Hooked_FirstPersonCharacter__CalculateJumpVerticalSpeed(FirstPersonCharacter* __this);
static void Hooked_EnemyHealth__Hit(EnemyHealth* __this, int32_t damage);
static int32_t Hooked_animalHealth__Hit(animalHealth* __this, int32_t damage);
static void Hooked_WeatherSystem__Update(WeatherSystem* __this);
static void Hooked_PlayerInventory__Update(PlayerInventory* __this);
static void Hooked_TreeHealth__DamageTree(TreeHealth* __this);
static bool Hooked_PlayerInventory__RemoveItem(PlayerInventory* __this, int32_t itemId, int32_t amount, bool allowAmountOverflow, bool shouldEquipPrevious);
static void Hooked_PlaneCrashController__Start(void* __this);
#ifdef FREE
static void Hooked_SteamSocket__Recv(void* __this, bool isServer);
#endif

struct Scene {
    static void* ActiveMB() { return *Scene__ActiveMB; }
    static Unity::GameObject* Yacht() { return *Scene__Yacht; }
    static Unity::Transform* SinkHoleCenter() { return *Scene__SinkHoleCenter; }
    static sceneTracker* SceneTracker() { return *Scene__SceneTracker; }
    static mutantController* MutantControler() { return *Scene__MutantControler; }
};

struct Cheats {
    static bool& Godmode() { return *Cheats__GodMode; }
    static bool& InfiniteEnergy() { return *Cheats__InfiniteEnergy; }
    static bool& Creative() { return *Cheats__Creative; }
    static bool& UnlimitedHairspray() { return *Cheats__UnlimitedHairspray; }
};

struct DebugConsole : Unity::Component {
    void _addClothingOutfitRandom() { 
        reinterpret_cast<void(*)(DebugConsole*)>(DebugConsole___addClothingOutfitRandom.Ptr())(this);
    }
    
    void _addAllStoryItems() { 
        reinterpret_cast<void(*)(DebugConsole*, void*)>(DebugConsole___addAllStoryItems.Ptr())(this, nullptr);
    }
    
    void _addAllItems() { 
        reinterpret_cast<void(*)(DebugConsole*, void*)>(DebugConsole___addAllItems.Ptr())(this, nullptr);
    }

    void _spawnenemy(System::String* mutantName) {
        reinterpret_cast<void(*)(DebugConsole*, System::String*)>(DebugConsole___spawnenemy.Ptr())(this, mutantName);
    }
};

struct LocalPlayer {
    static Unity::GameObject* GameObject() { return *LocalPlayer__GameObject; }
    static Unity::Transform* Transform() { return *LocalPlayer__Transform; }
    static PlayerStats* Stats() { return *LocalPlayer__Stats; }
    static PlayerInventory* Inventory() { return *LocalPlayer__Inventory; }
public:
    static bool IsInInventory() { return reinterpret_cast<bool(*)()>(LocalPlayer__get_IsInInventory.Ptr())(); }
};

struct mutantController {
    System::List<Unity::GameObject*>* activeCannibals() { return *(System::List<Unity::GameObject*>**)((char*)this + mutantController__activeCannibals); }
    System::List<Unity::GameObject*>* activeNetCannibals() { return *(System::List<Unity::GameObject*>**)((char*)this + mutantController__activeNetCannibals); }
};

struct AccountInfo {
    static bool UnlockAchievement(AchievementData* ach) {
        return reinterpret_cast<bool(*)(AchievementData*)>(AccountInfo__UnlockAchievement.Ptr())(ach);
    }
};

struct WeatherSystem {
    enum RainTypes { None, Light, Medium, Heavy };
    void TurnOn(RainTypes type) {
        void* args[1] = {&type};
        WeatherSystem__TurnOn.Invoke(this, args);
    }
};

struct SpawnPool {
    System::List<Unity::Transform*>* _spawned() { return *(System::List<Unity::Transform*>**)((char*)this + SpawnPool___spawned); }
    Unity::Transform* Spawn(System::String* prefabName, Unity::Vector3 pos, Unity::Quaternion rot) {
        void* args[3] = { prefabName, &pos, &rot };
        return reinterpret_cast<Unity::Transform*>(SpawnPool__Spawn.Invoke(this, args));
    }
};

struct SpawnPoolsDict {
    SpawnPool* get_Item(System::String* index) {
        void* args[1] = { index };
        return (SpawnPool*)SpawnPoolsDict__get_Item.Invoke(this, args);
    }
    bool ContainsKey(System::String* key) {
        void* args[1] = { key };
        return *(bool*)SpawnPoolsDict__ContainsKey.Invoke(this, args);
    }
};

struct PlayerStats {
    float& Fullness() { return *(float*)((char*)this + PlayerStats__Fullness); }
    float& Thirst() { return *(float*)((char*)this + PlayerStats__Thirst); }
    float& BatteryCharge() { return *(float*)((char*)this + PlayerStats__BatteryCharge); }
    FuelData* Fuel() { return *(FuelData**)((char*)this + PlayerStats__Fuel); }
};

struct FuelData {
    float& CurrentFuel() { 
        static int offset = Class::Find("Assembly-CSharp", "", "PlayerStats/FuelData")->FindField("CurrentFuel")->Offset();
        return *(float*)((char*)this + offset);
    }
    float& MaxFuelCapacity() {
        static int offset = Class::Find("Assembly-CSharp", "", "PlayerStats/FuelData")->FindField("MaxFuelCapacity")->Offset();
        return *(float*)((char*)this + offset);
    }
};

struct InventoryItem {
    int& _amount() { return *(int*)((char*)this + InventoryItem___amount); }
};

struct FirstPersonCharacter {
    bool& allowFallDamage() { return *(bool*)((char*)this + FirstPersonCharacter__allowFallDamage); }
    float& speed() { return *(float*)((char*)this + FirstPersonCharacter__speed); }
};

struct EnemyHealth {
    void Die() { EnemyHealth__Die.Invoke(this, nullptr); }
};

struct PlayerInventory {
    bool AddItem(int32_t itemId, int32_t amount, bool preventAutoEquip, bool fromCraftingCog) {
        void* args[] = { &itemId , &amount, &preventAutoEquip, &fromCraftingCog, nullptr };
        return *(bool*)PlayerInventory__AddItem.Invoke(this, args)->Unbox();
    }
public:
    System::Dictionary<int32_t, void*>* _inventoryItemViewsCache() { return *(System::Dictionary<int32_t, void*>**)((char*)this + PlayerInventory___inventoryItemViewsCache); }
};

struct sceneTracker {
    Unity::GameObject* planeCrash() { return *(Unity::GameObject**)((char*)this + sceneTracker__planeCrash); }
    System::List<Unity::Component*>* caveEntrances() { return *(System::List<Unity::Component*>**)((char*)this + sceneTracker__caveEntrances); }
};

struct Item {
    int32_t _maxAmount() const { return *(int32_t*)((char*)this + Item___maxAmount); }
    int32_t _id() const { return *(int32_t*)((char*)this + Item___id); }
    System::String* _name() const { return *(System::String**)((char*)this + Item___name); }
};

struct ItemDatabase {
    static ItemDatabase* _intance() { return *ItemDatabase___instance; }
public:
    System::Array<Item*>* _items() { return *(System::Array<Item*>**)((char*)this + ItemDatabase___items); }
};

struct BoltNetwork {
    static bool isRunning() { return reinterpret_cast<bool(*)()>(BoltNetwork__get_isRunning.Ptr())(); }
};

static void EnableCheat() {
    g_debugConsole = (DebugConsole*)DebugConsole::NewI(Class::Find("Assembly-CSharp", "TheForest", "DebugConsole"));
    g_light = Unity::GameObject::Ctor("PlayerLight98");
    g_light->GetTransform()->SetParent(LocalPlayer::Transform());
    Unity::Vector3 newPos = LocalPlayer::Transform()->GetPosition();
    newPos.y += 2.F;
    g_light->GetTransform()->SetPosition(newPos);
    g_light->SetActive(false);
    Unity::Light* light = (Unity::Light*)g_light->AddComponent(Class::Find("UnityEngine", "UnityEngine", "Light")->Type());
    light->SetIntensity(.5f);
    light->SetRange(1000.f);
    g_inGame = true;
}

void ModMenu::Initialize() {
    #define HAXSDK_FUNCTION(a, n, c, m, s)          c ## __ ## m = Class::Find(a, n, #c)->FindMethod(#m, s)
    #define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  c ## __ ## m = Class::Find(a, n, #c)->FindMethod(mn, s)
    #define HAXSDK_STATIC_FIELD(a, n, c, f, t)      c ## __ ## f = (t*)Class::Find(a, n, #c)->FindStaticField(#f)
    #define HAXSDK_FIELD_OFFSET(a, n, c, f)         c ## __ ## f = Class::Find(a, n, #c)->FindField(#f)->Offset()
    #include "cheat_data.h"

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)(PlayerStats__Update.Ptr()), Hooked_PlayerStats__Update);
    DetourAttach(&(PVOID&)(BatteryBasedLight__Update.Ptr()), Hooked_BatteryBasedLight__Update);
    DetourAttach(&(PVOID&)(PlayerStats__get_Warm.Ptr()), Hooked_PlayerStats__get_Warm);
    DetourAttach(&(PVOID&)(InventoryItem__Add.Ptr()), Hooked_InventoryItem__Add);
    DetourAttach(&(PVOID&)(FirstPersonCharacter__HandleLanded.Ptr()), Hooked_FirstPersonCharacter__HandleLanded);
    DetourAttach(&(PVOID&)(FirstPersonCharacter__HandleWalkingSpeedOptions.Ptr()), Hooked_FirstPersonCharacter__HandleWalkingSpeedOptions);
    DetourAttach(&(PVOID&)(FirstPersonCharacter__HandleRunningStaminaAndSpeed.Ptr()), Hooked_FirstPersonCharacter__HandleRunningStaminaAndSpeed);
    DetourAttach(&(PVOID&)(FirstPersonCharacter__CalculateJumpVerticalSpeed.Ptr()), Hooked_FirstPersonCharacter__CalculateJumpVerticalSpeed);
    DetourAttach(&(PVOID&)(EnemyHealth__Hit.Ptr()), Hooked_EnemyHealth__Hit);
    DetourAttach(&(PVOID&)(animalHealth__Hit.Ptr()), Hooked_animalHealth__Hit);
    DetourAttach(&(PVOID&)(LocalPlayer__Awake.Ptr()), Hooked_LocalPlayer__Awake);
    DetourAttach(&(PVOID&)(LocalPlayer__OnDestroy.Ptr()), Hooked_LocalPlayer__OnDestroy);
    DetourAttach(&(PVOID&)(WeatherSystem__Update.Ptr()), Hooked_WeatherSystem__Update);
    DetourAttach(&(PVOID&)(PlayerInventory__Update.Ptr()), Hooked_PlayerInventory__Update);
    DetourAttach(&(PVOID&)(TreeHealth__DamageTree.Ptr()), Hooked_TreeHealth__DamageTree);
    DetourAttach(&(PVOID&)(PlayerInventory__RemoveItem.Ptr()), Hooked_PlayerInventory__RemoveItem);
    DetourAttach(&(PVOID&)(PlaneCrashController__Start.Ptr()), Hooked_PlaneCrashController__Start);
#ifdef FREE
    DetourAttach(&(PVOID&)(SteamSocket__Recv.Ptr()), Hooked_SteamSocket__Recv);
#endif
    DetourTransactionCommit();

    if (Scene::ActiveMB())
        EnableCheat();
}

void HaxSdk::RenderImages() {
    g_textureSize = ImVec2(301.f, 77.f);
    g_BoostyTexture = HaxSdk::LoadTextureFromData(g_boostyImage, (int)g_textureSize.x, (int)g_textureSize.y);
}

static void AddText(const char* text, const ImVec2& pos, ImU32 col) {
    ImVec2 textSize = g_espFont->CalcTextSizeA(18, FLT_MAX, 0.0f, text);
    ImDrawList* pDrawList = ImGui::GetBackgroundDrawList();
    pDrawList->AddText(g_espFont, 18, ImVec2(pos.x - textSize.x / 2.F + 1.F, pos.y - 1.F), IM_COL32_BLACK, text);
    pDrawList->AddText(g_espFont, 18, ImVec2(pos.x - textSize.x / 2.F + 1.F, pos.y + 1.F), IM_COL32_BLACK, text);
    pDrawList->AddText(g_espFont, 18, ImVec2(pos.x - textSize.x / 2.F - 1.F, pos.y + 1.F), IM_COL32_BLACK, text);
    pDrawList->AddText(g_espFont, 18, ImVec2(pos.x - textSize.x / 2.F - 1.F, pos.y - 1.F), IM_COL32_BLACK, text);
    pDrawList->AddText(g_espFont, 18, ImVec2(pos.x - textSize.x / 2.F, pos.y), col, text);
}

static const char* ParseAnimalName(System::String* string) {
    const wchar_t* wname = string->Data();
    if (wcsncmp(wname, L"deer", 4) == 0)
        return LOCALE_DEER[g_curLocale];
    if (wcsncmp(wname, L"rabbit", 6) == 0)
        return LOCALE_RABBIT[g_curLocale];
    if (wcsncmp(wname, L"bat", 3) == 0)
        return LOCALE_BAT[g_curLocale];
    if (wcsncmp(wname, L"bird", 4) == 0)
        return LOCALE_BIRD[g_curLocale];
    if (wcsncmp(wname, L"goose", 5) == 0)
        return LOCALE_GOOSE[g_curLocale];
    if (wcsncmp(wname, L"fish", 4) == 0 || wcsncmp(wname, L"Fish", 4) == 0)
        return LOCALE_FISH[g_curLocale];
    if (wcsncmp(wname, L"oyster", 6) == 0)
        return LOCALE_OYSTER[g_curLocale];
    if (wcsncmp(wname, L"lizard", 6) == 0)
        return LOCALE_LIZARD[g_curLocale];
    if (wcsncmp(wname, L"raccoon", 7) == 0)
        return LOCALE_RACCOON[g_curLocale];
    if (wcsncmp(wname, L"seagull", 7) == 0)
        return LOCALE_SEAGULL[g_curLocale];
    if (wcsncmp(wname, L"squirrel", 8) == 0)
        return LOCALE_SQUIRREL[g_curLocale];
    if (wcsncmp(wname, L"tortoise", 8) == 0)
        return LOCALE_TORTOISE[g_curLocale];
    if (wcsncmp(wname, L"turtle", 6) == 0)
        return LOCALE_TURTLE[g_curLocale];
    if (wcsncmp(wname, L"boar", 4) == 0)
        return LOCALE_BOAR[g_curLocale];
    if (wcsncmp(wname, L"shark", 5) == 0)
        return LOCALE_SHARK[g_curLocale];
    if (wcsncmp(wname, L"crocodile", 9) == 0)
        return LOCALE_CROCODILE[g_curLocale];
    return nullptr;
}

static const char* ParseCannibalName(System::String* string) {
    const wchar_t* wname = string->Data();
    if (wcsncmp(wname, L"mutant_female", 13) == 0 || wcsncmp(wname, L"mutant_male", 11) == 0)
        return LOCALE_CANNIBAL[g_curLocale];
    if (wcsncmp(wname, L"male_creepy", 11) == 0)
        return LOCALE_ARMSY[g_curLocale];
    if (wcsncmp(wname, L"female_creepy", 13) == 0)
        return LOCALE_VIRGINIA[g_curLocale];
    if (wcsncmp(wname, L"mutant_creepy_fat", 17) == 0)
        return LOCALE_COWMAN[g_curLocale];
    if (wcsncmp(wname, L"girl", 4) == 0)
        return LOCALE_COWMAN[g_curLocale];
    return string->UTF8();
}

void HaxSdk::RenderBackground() {
    if (g_inGame && !LocalPlayer::IsInInventory()) {
        Unity::Vector3 playerPos = LocalPlayer::Transform()->GetPosition();
        Unity::Camera* pCamera = *LocalPlayer__MainCam;

        // Animals ESP
        if (g_animalEsp) {
            SpawnPoolsDict* pSpawnPoolsDict = *PoolManager__Pools;
            System::String* poolName = BoltNetwork::isRunning() ? System::String::New("creatures_net") : System::String::New("creatures");
            if (pSpawnPoolsDict->ContainsKey(poolName)) {
                System::List<Unity::Transform*>* pCreatures = pSpawnPoolsDict->get_Item(poolName)->_spawned();
                Unity::Transform** pCreaturesData = pCreatures->Data();
                for (int32_t i = 0; i < pCreatures->Lenght(); ++i) {
                    Unity::GameObject* pCreature = pCreaturesData[i]->GetGameObject();
                    if (pCreature) {
                        Unity::Vector3 creaturePos = pCreature->GetTransform()->GetPosition();
                        Unity::Vector3 screenPos = pCamera->WorldToScreenPoint(creaturePos);
                        if (screenPos.z > 0) {
                            const char* parsedName = ParseAnimalName(pCreature->GetName());
                            if (parsedName)
                                AddText(parsedName, ImVec2(screenPos.x, globals::g_screenHeight - screenPos.y - 20.f), 0xFF00FF00);
                        }
                    }
                }
            }
        }

        // Cannibals ESP
        if (g_cannibalEsp) {
            System::List<Unity::GameObject*>* pActiveCannibals = BoltNetwork::isRunning() ? 
                Scene::MutantControler()->activeNetCannibals() : Scene::MutantControler()->activeCannibals();
            Unity::GameObject** pCannibalsData = pActiveCannibals->Data();
            for (int32_t i = 0; i < pActiveCannibals->Lenght(); ++i) {
                Unity::GameObject* pCannibal = pCannibalsData[i];
                Unity::Vector3 enemyPos = pCannibal->GetTransform()->GetPosition();
                Unity::Vector3 enemyScreenPos = pCamera->WorldToScreenPoint(enemyPos);
                if (enemyScreenPos.z > 0)
                    AddText(ParseCannibalName(pCannibal->GetName()), ImVec2(enemyScreenPos.x, globals::g_screenHeight - enemyScreenPos.y - 20.f), 0xFF0000FF);
            }
        }

#ifdef PATRON
        // Locations ESP
        if (g_locationsEsp) {
            sceneTracker* pSceneTracker = Scene::SceneTracker();
            Unity::Vector3 yachtScreenPos = pCamera->WorldToScreenPoint(Scene::Yacht()->GetTransform()->GetPosition());
            if (yachtScreenPos.z > 0)
                AddText(LOCALE_YACHT[g_curLocale], ImVec2(yachtScreenPos.x, globals::g_screenHeight - yachtScreenPos.y - 20.f), 0xFF00FFFF);

            Unity::Vector3 planeWorldPos = pSceneTracker->planeCrash()->GetTransform()->GetPosition();
            Unity::Vector3 planeScreenPos = pCamera->WorldToScreenPoint(planeWorldPos);
            if (planeScreenPos.z > 0) {
                float dist = Unity::Vector3::Distance(playerPos, planeWorldPos);
                if (dist > 1.F) {
                    char buff[30];
                    snprintf(buff, sizeof(buff), "%s [%.0fm]", LOCALE_PLANE[g_curLocale], dist);
                    AddText(buff, ImVec2(planeScreenPos.x, globals::g_screenHeight - planeScreenPos.y - 20.f), 0xFF00FFFF);
                }
            }

            Unity::Vector3 pitWorldPos = Scene::SinkHoleCenter()->GetPosition();
            Unity::Vector3 pitScreenPos = pCamera->WorldToScreenPoint(pitWorldPos);
            if (pitScreenPos.z > 0) {
                float dist = Unity::Vector3::Distance(playerPos, pitWorldPos);
                if (dist > 1.F) {
                    char buff[30];
                    snprintf(buff, sizeof(buff), "%s [%.0fm]", LOCALE_PIT[g_curLocale], dist);
                    AddText(buff, ImVec2(pitScreenPos.x, globals::g_screenHeight - pitScreenPos.y - 20.f), 0xFF00FFFF);
                }
             }

            System::List<Unity::Component*>* pCaves = pSceneTracker->caveEntrances();
            Unity::Component** pCavesData = pCaves->Data();
            for (size_t i = 0; i < pCaves->Lenght(); ++i) {
                Unity::Vector3 caveWorldPos = pCavesData[i]->GetTransform()->GetPosition();
                Unity::Vector3 caveScreenPos = pCamera->WorldToScreenPoint(caveWorldPos);
                if (caveScreenPos.z > 0) {
                    float dist = Unity::Vector3::Distance(playerPos, caveWorldPos);
                    if (dist > 1.F) {
                        char buff[30];
                        snprintf(buff, sizeof(buff), "%s [%.0fm]", LOCALE_CAVE[g_curLocale], dist);
                        AddText(buff, ImVec2(caveScreenPos.x, globals::g_screenHeight - caveScreenPos.y), 0xFF0045FF);
                    }
                }
            }
        }
#endif
    }
}

static std::vector<Item*> GetAllItems(std::vector<const char*>& itemsNames) {
    System::Array<Item*>* pItems = ItemDatabase::_intance()->_items();
    Item** pItemsData = pItems->Data();
    size_t nItems = pItems->Length();
    System::Dictionary<int32_t, void*>* pItemViews = LocalPlayer::Inventory()->_inventoryItemViewsCache();

    std::vector<Item*> result;
    result.reserve(nItems);
    for (size_t i = 0; i < nItems; ++i) {
        if (pItemsData[i]->_maxAmount() >= 0 && pItemViews->ContainsKey(pItemsData[i]->_id())) {
            result.push_back(pItemsData[i]);
        }
    }

    std::sort(result.begin(), result.end(), [](Item* a, Item* b) { return wcscmp(a->_name()->Data(), b->_name()->Data()) < 0; });
    itemsNames.reserve(result.size());
    for (Item* pItem : result)
        itemsNames.push_back(pItem->_name()->UTF8());
    return result;
}

static void HelpMarker(const char* desc)
{
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(0xC9, 0xC9, 0xC9, 0xFF));
    ImGui::TextDisabled("(?)");
    ImGui::PopStyleColor();
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

static void WarningMarker(const char* desc) {
    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, IM_COL32(0xFF, 0xFF, 0x00, 0xFF));
    ImGui::TextDisabled("(!)");
    ImGui::PopStyleColor();
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

static void RenderPlayerTab() {
    if (ImGui::BeginTabItem(LOCALE_PLAYER[g_curLocale])) {
        ImGui::SeparatorText(LOCALE_VITALS[g_curLocale]);
        ImGui::Checkbox(LOCALE_GODMODE[g_curLocale], Cheats__GodMode);
        ImGui::Checkbox(LOCALE_INFENERGY[g_curLocale], Cheats__InfiniteEnergy);
        ImGui::Checkbox(LOCALE_FULLNESS[g_curLocale], &g_maxFullness);
        ImGui::Checkbox(LOCALE_THIRST[g_curLocale], &g_noThirst);
        ImGui::Checkbox(LOCALE_COLD[g_curLocale], &g_noCold);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::SeparatorText(LOCALE_MOVEMENT[g_curLocale]);
        ImGui::SliderFloat(LOCALE_MOVACCEL[g_curLocale], &g_speedMult, 1.F, 5.F, "%.1f");
        ImGui::SliderFloat(LOCALE_JUMPAMPL[g_curLocale], &g_jumpMult, 1.F, 15.F, "%.1f");
        ImGui::Checkbox(LOCALE_FALLDMG[g_curLocale], &g_noFallDamage);
        HelpMarker(LOCALE_TIP_FALL[g_curLocale]);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::SeparatorText(LOCALE_MISC[g_curLocale]);
        ImGui::Combo(LOCALE_ARMOR[g_curLocale], &g_curArmor, LOCALE_ARMORS[g_curLocale], 6);
        if (ImGui::Button(LOCALE_GET_ARMOR[g_curLocale]))
            g_forceArmor = true;
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
#ifdef PATRON
        ImGui::Checkbox(LOCALE_TREECUT[g_curLocale], &g_fastTreeCut);
        HelpMarker(LOCALE_TIP_TREECUT[g_curLocale]);
#endif
        if (ImGui::Button(LOCALE_RANDOUTFIT[g_curLocale]))
            g_forceRandomOutfit = true;
#ifdef PATRON
        if (ImGui::Button(LOCALE_ACHIEVS[g_curLocale]))
            g_forceAchievements = true;
#endif
        ImGui::EndTabItem();
    }
}

static void RenderItemsTab() {
    if (ImGui::BeginTabItem(LOCALE_ITEMS[g_curLocale])) {
        ImGui::SeparatorText(LOCALE_GENERAL[g_curLocale]);
        ImGui::Checkbox(LOCALE_INFBATTERY[g_curLocale], &g_infiniteBattery);
        ImGui::Checkbox(LOCALE_BUILDING[g_curLocale], Cheats__Creative);
        ImGui::Checkbox(LOCALE_INFFUEL[g_curLocale], &g_maxFuel);
        ImGui::Checkbox(LOCALE_HAIRSPRAY[g_curLocale], Cheats__UnlimitedHairspray);
        ImGui::Checkbox(LOCALE_INFCONSUME[g_curLocale], &g_infConsumables);
        HelpMarker(LOCALE_TIP_CONSUMABLES[g_curLocale]);
#ifdef PATRON
        if (ImGui::Button(LOCALE_STORYITEMS[g_curLocale]))
            g_forceStoryItems = true;
#endif
        if (ImGui::Button(LOCALE_ALLITEMS[g_curLocale]))
            g_forceAllItems = true;
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        static int curItem = 0;
        static std::vector<const char*> itemsNames;
        static std::vector<Item*> items = GetAllItems(itemsNames);
        ImGui::Combo(LOCALE_ITEM[g_curLocale], &curItem, itemsNames.data(), (int)itemsNames.size());
        if (ImGui::Button(LOCALE_GET_ITEM[g_curLocale])) {
            g_forceItemId = items[curItem]->_id();
            g_forceItem = true;
        }
        ImGui::EndTabItem();
    }
}

static void RenderWorldTab() {
    if (ImGui::BeginTabItem(LOCALE_WORLD[g_curLocale])) {
#ifdef PATRON
        ImGui::Checkbox(LOCALE_LOCATIONSESP[g_curLocale], &g_locationsEsp);
        HelpMarker(LOCALE_TIP_ESP[g_curLocale]);
#endif
        if (ImGui::Checkbox(LOCALE_SPEEDCLOCK[g_curLocale], &g_speedUpGame))
            *TheForestAtmosphere__GameTimeScale = g_speedUpGame ? 200.F : 1.F;
        if (ImGui::Checkbox(LOCALE_DOLIGHT[g_curLocale], &g_doLight))
            g_light->SetActive(g_doLight);
        ImGui::Combo(LOCALE_RAIN[g_curLocale], &g_forceRainType, LOCALE_RAIN_TYPES[g_curLocale], 4);
        if (ImGui::Button(LOCALE_SET[g_curLocale]))
            g_forceRain = true;
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::SeparatorText(LOCALE_CREATURES[g_curLocale]);
        if (ImGui::Checkbox(LOCALE_INVISIBLE[g_curLocale], &g_invisible))
            LocalPlayer::GameObject()->SetLayer(g_invisible ? 31 : 18);
        ImGui::Checkbox(LOCALE_OHK[g_curLocale], &g_ohk);
        ImGui::Checkbox(LOCALE_CANNESP[g_curLocale], &g_cannibalEsp);
        ImGui::Combo(LOCALE_ENEMY[g_curLocale], &g_curEnemy, LOCALE_ENEMIES[g_curLocale], 13);
        WarningMarker(LOCALE_TIP_ENEMIES[g_curLocale]);
        if (ImGui::Button(LOCALE_SPAWN_ENEMY[g_curLocale]))
            g_forceEnemySpawn = true;
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        ImGui::Checkbox(LOCALE_ANIMESP[g_curLocale], &g_animalEsp);
        ImGui::Combo(LOCALE_ANIMAL[g_curLocale], &g_curAnimal, LOCALE_ANIMALS[g_curLocale], 8);
        if (ImGui::Button(LOCALE_SPAWN_ANIMAL[g_curLocale]))
            g_forceAnimalSpawn = true;
        ImGui::EndTabItem();
    }
}

static void RenderSettingsTab() {
    if (ImGui::BeginTabItem(LOCALE_SETTINGS[g_curLocale])) {
        static const char* items[]{ "English", reinterpret_cast<const char*>(u8"Русский") };
        ImGui::Combo(LOCALE_LANGUAGE[g_curLocale], &g_curLocale, items, IM_ARRAYSIZE(items));
        ImGui::EndTabItem();
    }
}

static void RenderMainMenuTab() {
    if (ImGui::BeginTabItem(LOCALE_MENU[g_curLocale])) {
#ifdef FREE
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0xFF, 0x00, 0x00, 0xFF));
        ImGui::Text(LOCALE_NONETWORK[g_curLocale]);
        ImGui::PopStyleColor();
#endif
        ImGui::Checkbox(LOCALE_SKIPPLANE[g_curLocale], &g_skipPlaneCrash);
        WarningMarker(LOCALE_TIP_PLANE[g_curLocale]);
        ImGui::Text(LOCALE_PREMENU[g_curLocale]);
        ImGui::EndTabItem();
#ifdef FREE
        ImGui::Dummy(ImVec2(0, 10.F));
        ImGui::Text(LOCALE_BOOSTY[g_curLocale]);
#endif
    }
}

void HaxSdk::RenderMenu() {
    ImGui::SetNextWindowSize(g_modMenuSize, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowBgAlpha(1);
    ImGui::Begin("The Forest Mod Menu [by Sacracia]", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::BeginTabBar("TheForest#TopBar", ImGuiTabBarFlags_NoTabListScrollingButtons);
    if (g_inGame) {
        RenderPlayerTab();
        RenderItemsTab();
        RenderWorldTab();
    }
    else {
        RenderMainMenuTab();
    }
    RenderSettingsTab();
    ImGui::EndTabBar();
    if (g_BoostyTexture) {
        ImGui::SetCursorPos(ImVec2(71.f, 504.f));
        if (ImGui::ImageButton((ImTextureID)g_BoostyTexture, g_textureSize, ImVec2(0,0), ImVec2(1, 1), 0))
            ShellExecute(0, 0, "http://www.google.com", 0, 0, SW_SHOW);
    }
    ImGui::End();
}

static void Hooked_LocalPlayer__Awake(LocalPlayer* __this) {
    reinterpret_cast<void(*)(LocalPlayer*)>(LocalPlayer__Awake.Ptr())(__this);
    EnableCheat();
}

static void Hooked_LocalPlayer__OnDestroy(LocalPlayer* __this) {
    g_inGame = false;
    g_animalEsp = g_cannibalEsp = g_locationsEsp = false;
    g_invisible = false;
    Unity::Object::Destroy(g_light);
    reinterpret_cast<void(*)(LocalPlayer*)>(LocalPlayer__OnDestroy.Ptr())(__this);
}

static void Hooked_PlayerStats__Update(PlayerStats* __this) {
    if (g_infiniteBattery)
        __this->BatteryCharge() = 100.f;
    if (g_maxFullness)
        __this->Fullness() = 100.f;
    if (g_noThirst)
        __this->Thirst() = 0.f;
    if (g_maxFuel) {
        FuelData* pFuel = __this->Fuel();
        pFuel->CurrentFuel() = pFuel->MaxFuelCapacity();
    }
    if (g_forceArmor) {
        static int32_t armorIds[] = { 1, 2, 4, 8, 9, 16 };
        for (int i = 0; i < 10; ++i)
            reinterpret_cast<void(*)(PlayerStats*, int32_t)>(PlayerStats__AddArmorVisible.Ptr())(__this, armorIds[g_curArmor]);
        g_forceArmor = false;
    }
    reinterpret_cast<void(*)(PlayerStats*)>(PlayerStats__Update.Ptr())(__this);
}

static bool Hooked_PlayerStats__get_Warm(PlayerStats* __this) {
    return g_noCold ? true : reinterpret_cast<bool(*)(PlayerStats*)>(PlayerStats__get_Warm.Ptr())(__this);
}

static void Hooked_BatteryBasedLight__Update(void* __this) {
    if (g_infiniteBattery)
        LocalPlayer::Stats()->BatteryCharge() = 100.f;
    reinterpret_cast<void(*)(void*)>(BatteryBasedLight__Update.Ptr())(__this);
}

static int32_t Hooked_InventoryItem__Add(InventoryItem* __this, int32_t amount, bool isEquiped) {
    if (g_unlimitedItems) {
        __this->_amount() += amount;
        return 0;
    }
    return reinterpret_cast<int32_t(*)(InventoryItem*,int32_t,bool)>(InventoryItem__Add.Ptr())(__this, amount, isEquiped);
}

static void Hooked_FirstPersonCharacter__HandleLanded(FirstPersonCharacter* __this) {
    if (g_noFallDamage)
        __this->allowFallDamage() = false;
    reinterpret_cast<void(*)(FirstPersonCharacter*)>(FirstPersonCharacter__HandleLanded.Ptr())(__this);
}

static void Hooked_FirstPersonCharacter__HandleWalkingSpeedOptions(FirstPersonCharacter* __this) {
    reinterpret_cast<void(*)(FirstPersonCharacter*)>(FirstPersonCharacter__HandleWalkingSpeedOptions.Ptr())(__this);
    __this->speed() *= g_speedMult;
}

static void Hooked_FirstPersonCharacter__HandleRunningStaminaAndSpeed(FirstPersonCharacter* __this) {
    reinterpret_cast<void(*)(FirstPersonCharacter*)>(FirstPersonCharacter__HandleRunningStaminaAndSpeed.Ptr())(__this);
    __this->speed() *= g_speedMult;
}

static float Hooked_FirstPersonCharacter__CalculateJumpVerticalSpeed(FirstPersonCharacter* __this) {
    return reinterpret_cast<float(*)(FirstPersonCharacter*)>(FirstPersonCharacter__CalculateJumpVerticalSpeed.Ptr())(__this) * g_jumpMult;
}

static void Hooked_EnemyHealth__Hit(EnemyHealth* __this, int32_t damage) {
    if (g_ohk) {
        __this->Die();
        return;
    }
    reinterpret_cast<void(*)(EnemyHealth*,int32_t)>(EnemyHealth__Hit.Ptr())(__this, damage);
}

static int32_t Hooked_animalHealth__Hit(animalHealth* __this, int32_t damage) {
    if (g_ohk) {
        reinterpret_cast<int32_t(*)(animalHealth*)>(animalHealth__Die.Ptr())(__this);
        return 1;
    }
    return reinterpret_cast<int32_t(*)(animalHealth*, int32_t)>(animalHealth__Hit.Ptr())(__this, damage);
}

static void Hooked_WeatherSystem__Update(WeatherSystem* __this) {
    if (g_forceRain) {
        reinterpret_cast<void(*)(WeatherSystem*,int32_t)>(WeatherSystem__TurnOn.Ptr())(__this, g_forceRainType);
        g_forceRain = false;
    }
    if (g_forceAchievements) {
        System::Array<AchievementData*>* achievements = *Achievements__Data;
        for (int i = 0; i < achievements->Length(); ++i)
            AccountInfo::UnlockAchievement(achievements->Data()[i]);
        g_forceAchievements = false;
    }
    reinterpret_cast<void(*)(WeatherSystem*)>(WeatherSystem__Update.Ptr())(__this);
}

static void Hooked_TreeHealth__DamageTree(TreeHealth* __this) {
    if (g_fastTreeCut) {
        reinterpret_cast<void(*)(TreeHealth*, float)>(TreeHealth__Explosion.Ptr())(__this, 0);
        return;
    }
    reinterpret_cast<void(*)(TreeHealth*)>(TreeHealth__DamageTree.Ptr())(__this);
}

static void Hooked_PlayerInventory__Update(PlayerInventory* __this) {
    if (g_forceAllItems) {
        g_debugConsole->_addAllItems();
        g_forceAllItems = false;
    }
    if (g_forceStoryItems) {
        g_debugConsole->_addAllStoryItems();
        g_forceStoryItems = false;
    }
    if (g_forceItem) {
        __this->AddItem(g_forceItemId, 1, false, false);
        g_forceItem = false;
    }
    if (g_forceRandomOutfit) {
        g_debugConsole->_addClothingOutfitRandom();
        g_forceRandomOutfit = false;
    }
    if (g_forceEnemySpawn) {
        g_debugConsole->_spawnenemy(System::String::New(enemiesInternalNames[g_curEnemy]));
        g_forceEnemySpawn = false;
    }
    if (g_forceAnimalSpawn && !BoltNetwork::isRunning()) {
        System::String* prefabName = System::String::New(animalsInternalNames[g_curAnimal]);
        SpawnPoolsDict* pSwapPoolsDict = *PoolManager__Pools;
        SpawnPool* pSpawnPool = pSwapPoolsDict->get_Item(System::String::New("creatures"));
        Unity::Transform* pPlayerTransform = LocalPlayer::Transform();
        Unity::Transform* pTransform = pSpawnPool->Spawn(prefabName, pPlayerTransform->GetPosition() + pPlayerTransform->GetForward() * 2.F, Unity::Quaternion::GetIdentity());
        static System::Type* pCoopAnimalServerType = Class::Find("Assembly-CSharp", "", "CoopAnimalServer")->Type();
        if (pTransform && pTransform->GetGameObject()->GetComponent(pCoopAnimalServerType) != nullptr)
            reinterpret_cast<void(*)(void*,Unity::GameObject*)>(AnimalSpawnController__AttachAnimalToNetwork.Ptr())(nullptr, pTransform->GetGameObject());
        g_forceAnimalSpawn = false;
    }
    reinterpret_cast<void(*)(PlayerInventory*)>(PlayerInventory__Update.Ptr())(__this);
}

static bool Hooked_PlayerInventory__RemoveItem(PlayerInventory* __this, int32_t itemId, int32_t amount, bool allowAmountOverflow, bool shouldEquipPrevious) {
    return g_infConsumables ? true : reinterpret_cast<bool(*)(PlayerInventory*, int32_t, int32_t, bool, bool)>(PlayerInventory__RemoveItem.Ptr())(__this, itemId, amount, allowAmountOverflow, shouldEquipPrevious);
}

static void Hooked_PlaneCrashController__Start(void* __this) {
    reinterpret_cast<void(*)(void*)>(PlaneCrashController__Start.Ptr())(__this);
    if (g_skipPlaneCrash)
        reinterpret_cast<void(*)(void*)>(PlaneCrashController__DisablePlaneCrash.Ptr())(__this);
}

#ifdef FREE
static void Hooked_SteamSocket__Recv(void* __this, bool isServer) {
    return;
}
#endif

void HaxSdk::ApplyStyle() {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    g_sizeScale = (float)desktop.bottom / 1080;
    float fontSize = std::round(13 * g_sizeScale);
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryTTF(g_fTrebuc, sizeof(g_fTrebuc), fontSize, NULL, io.Fonts->GetGlyphRangesCyrillic());
    g_espFont = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 32.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    g_modMenuSize.x *= g_sizeScale;
    g_modMenuSize.y *= g_sizeScale;

    ImGuiStyle* styles = &ImGui::GetStyle();

    auto colors = styles->Colors;
    colors[ImGuiCol_Border]                 = ImGui::ColorConvertU32ToFloat4_2(0x26383FFF);
    colors[ImGuiCol_BorderShadow]           = ImGui::ColorConvertU32ToFloat4_2(0x33333300);
    colors[ImGuiCol_Button]                 = ImGui::ColorConvertU32ToFloat4_2(0x23303DFF);
    colors[ImGuiCol_ButtonActive]           = ImGui::ColorConvertU32ToFloat4_2(0x474968FF);
    colors[ImGuiCol_ButtonHovered]          = ImGui::ColorConvertU32ToFloat4_2(0x444C70FF);
    colors[ImGuiCol_CheckMark]              = ImGui::ColorConvertU32ToFloat4_2(0xA5BCDBFF);
    colors[ImGuiCol_ChildBg]                = ImGui::ColorConvertU32ToFloat4_2(0x1E262BFF);
    colors[ImGuiCol_DragDropTarget]         = ImGui::ColorConvertU32ToFloat4_2(0xFFFF00E5);
    colors[ImGuiCol_FrameBg]                = ImGui::ColorConvertU32ToFloat4_2(0x2D3F44FF);
    colors[ImGuiCol_FrameBgActive]          = ImGui::ColorConvertU32ToFloat4_2(0x30383DFF);
    colors[ImGuiCol_FrameBgHovered]         = ImGui::ColorConvertU32ToFloat4_2(0x26303DFF);
    colors[ImGuiCol_Header]                 = ImGui::ColorConvertU32ToFloat4_2(0x0000003D);
    colors[ImGuiCol_HeaderActive]           = ImGui::ColorConvertU32ToFloat4_2(0x0070EAFF);
    colors[ImGuiCol_HeaderHovered]          = ImGui::ColorConvertU32ToFloat4_2(0x1E2833CC);
    colors[ImGuiCol_MenuBarBg]              = ImGui::ColorConvertU32ToFloat4_2(0x1E232DFF);
    colors[ImGuiCol_ModalWindowDimBg]       = ImGui::ColorConvertU32ToFloat4_2(0xCCCCCC59);
    colors[ImGuiCol_NavHighlight]           = ImGui::ColorConvertU32ToFloat4_2(0x4296F9FF);
    colors[ImGuiCol_NavWindowingDimBg]      = ImGui::ColorConvertU32ToFloat4_2(0xCCCCCC33);
    colors[ImGuiCol_NavWindowingHighlight]  = ImGui::ColorConvertU32ToFloat4_2(0xFFFFFFB2);
    colors[ImGuiCol_PlotHistogram]          = ImGui::ColorConvertU32ToFloat4_2(0xE5B200FF);
    colors[ImGuiCol_PlotHistogramHovered]   = ImGui::ColorConvertU32ToFloat4_2(0xFF9900FF);
    colors[ImGuiCol_PlotLines]              = ImGui::ColorConvertU32ToFloat4_2(0x9B9B9BFF);
    colors[ImGuiCol_PlotLinesHovered]       = ImGui::ColorConvertU32ToFloat4_2(0xFF6D59FF);
    colors[ImGuiCol_PopupBg]                = ImGui::ColorConvertU32ToFloat4_2(0x14161CEF);
    colors[ImGuiCol_ResizeGrip]             = ImGui::ColorConvertU32ToFloat4_2(0xA3C9F93F);
    colors[ImGuiCol_ResizeGripActive]       = ImGui::ColorConvertU32ToFloat4_2(0x6D8CB2F2);
    colors[ImGuiCol_ResizeGripHovered]      = ImGui::ColorConvertU32ToFloat4_2(0xA5BFDDAA);
    colors[ImGuiCol_ScrollbarBg]            = ImGui::ColorConvertU32ToFloat4_2(0x1C1C1C63);
    colors[ImGuiCol_ScrollbarGrab]          = ImGui::ColorConvertU32ToFloat4_2(0x875E5EFF);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImGui::ColorConvertU32ToFloat4_2(0x8E1919FF);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImGui::ColorConvertU32ToFloat4_2(0x7C3A3AFF);
    colors[ImGuiCol_Separator]              = ImGui::ColorConvertU32ToFloat4_2(0x333F49FF);
    colors[ImGuiCol_SeparatorActive]        = ImGui::ColorConvertU32ToFloat4_2(0x6B91BFFF);
    colors[ImGuiCol_SeparatorHovered]       = ImGui::ColorConvertU32ToFloat4_2(0x4F7299C6);
    colors[ImGuiCol_SliderGrab]             = ImGui::ColorConvertU32ToFloat4_2(0x5977ADFF);
    colors[ImGuiCol_SliderGrabActive]       = ImGui::ColorConvertU32ToFloat4_2(0xADCCFFFF);
    colors[ImGuiCol_Tab]                    = ImGui::ColorConvertU32ToFloat4_2(0x1C262BFF);
    colors[ImGuiCol_TabActive]              = ImGui::ColorConvertU32ToFloat4_2(0x333F49FF);
    colors[ImGuiCol_TabHovered]             = ImGui::ColorConvertU32ToFloat4_2(0x969696CC);
    colors[ImGuiCol_TabUnfocused]           = ImGui::ColorConvertU32ToFloat4_2(0x1C262BFF);
    colors[ImGuiCol_TabUnfocusedActive]     = ImGui::ColorConvertU32ToFloat4_2(0x1C262BFF);
    colors[ImGuiCol_TableBorderLight]       = ImGui::ColorConvertU32ToFloat4_2(0x3A3A3FFF);
    colors[ImGuiCol_TableBorderStrong]      = ImGui::ColorConvertU32ToFloat4_2(0x4F4F59FF);
    colors[ImGuiCol_TableHeaderBg]          = ImGui::ColorConvertU32ToFloat4_2(0x303033FF);
    colors[ImGuiCol_TableRowBg]             = ImGui::ColorConvertU32ToFloat4_2(0x333F49FF);
    colors[ImGuiCol_TableRowBgAlt]          = ImGui::ColorConvertU32ToFloat4_2(0x1C262BFF);
    colors[ImGuiCol_Text]                   = ImGui::ColorConvertU32ToFloat4_2(0xF2F4F9FF);
    colors[ImGuiCol_TextDisabled]           = ImGui::ColorConvertU32ToFloat4_2(0x2B353DFF);
    colors[ImGuiCol_TextSelectedBg]         = ImGui::ColorConvertU32ToFloat4_2(0x4296F959);
    colors[ImGuiCol_TitleBg]                = ImGui::ColorConvertU32ToFloat4_2(0x232D38A5);
    colors[ImGuiCol_TitleBgActive]          = ImGui::ColorConvertU32ToFloat4_2(0x212830FF);
    colors[ImGuiCol_TitleBgCollapsed]       = ImGui::ColorConvertU32ToFloat4_2(0x26262682);
    colors[ImGuiCol_WindowBg]               = ImGui::ColorConvertU32ToFloat4_2(0x1E2623FF);

    //Styles
    styles->Alpha                           = 1.0;
    styles->AntiAliasedFill                 = true;
    styles->AntiAliasedLines                = true;
    styles->AntiAliasedLinesUseTex          = true;
    styles->ButtonTextAlign                 = ImVec2(0.5, 0.5);
    styles->CellPadding                     = ImVec2(4.0, 2.0);
    styles->ChildBorderSize                 = 1.0;
    styles->ChildRounding                   = 5.0;
    styles->CircleTessellationMaxError      = 0.30000001192092896;
    styles->ColorButtonPosition             = ImGuiDir_Right;
    styles->ColumnsMinSpacing               = 6.0;
    styles->CurveTessellationTol            = 1.25;
    styles->DisabledAlpha                   = 0.6000000238418579;
    styles->DisplaySafeAreaPadding          = ImVec2(3.0, 3.0);
    styles->DisplayWindowPadding            = ImVec2(19.0, 19.0);
    styles->FrameBorderSize                 = 0.0;
    styles->FramePadding                    = ImVec2(4.0, 3.0);
    styles->FrameRounding                   = 4.0;
    styles->GrabMinSize                     = 10.0;
    styles->GrabRounding                    = 4.0;
    styles->IndentSpacing                   = 21.0;
    styles->ItemInnerSpacing                = ImVec2(4.0, 4.0);
    styles->ItemSpacing                     = ImVec2(8.0, 4.0);
    styles->LogSliderDeadzone               = 4.0;
    styles->MouseCursorScale                = 1.0;
    styles->PopupBorderSize                 = 1.0;
    styles->PopupRounding                   = 0.0;
    styles->ScrollbarRounding               = 9.0;
    styles->ScrollbarSize                   = 14.0;
    styles->SelectableTextAlign             = ImVec2(0.0, 0.0);
    styles->TabBorderSize                   = 0.0;
    styles->TabMinWidthForCloseButton       = 0.0;
    styles->TabRounding                     = 4.0;
    styles->TouchExtraPadding               = ImVec2(0.0, 0.0);
    styles->WindowBorderSize                = 1.0;
    styles->WindowMenuButtonPosition        = ImGuiDir_Left;
    styles->WindowMinSize                   = ImVec2(32.0, 32.0);
    styles->WindowPadding                   = ImVec2(8.0, 8.0);
    styles->WindowRounding                  = 0.0;
    styles->WindowTitleAlign                = ImVec2(0.0, 0.5);
}