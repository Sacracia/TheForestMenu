#include "cheat.h"

#include "haxsdk/haxsdk.h"
#include "haxsdk/third_party/imgui/imgui.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "haxsdk/third_party/detours/x64/detours.h"
#include "font.h"

struct mutantController;
struct Scene;
struct Cheats;
struct DebugConsole;
struct AchievementData;
struct WeatherSystem;
struct SpawnPoolsDict;
struct FirstPersonCharacter;
struct PlayerStats;
struct InventoryItem;
struct LocalPlayer;

#define HAXSDK_FUNCTION(a, n, c, m, s)          static Method c ## __ ## m
#define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  static Method c ## __ ## m
#define HAXSDK_STATIC_FIELD(a, n, c, f, t)      static t* c ## __ ## f
#define HAXSDK_FIELD_OFFSET(a, n, c, f)         static int c ## __ ## f
#include "cheat_data.h"

static ImFont* g_espFont;
static bool g_inGame = false;
static Unity::Camera* g_camera;
static bool g_infiniteBattery = false;
static bool g_maxFullness = false;
static bool g_noThirst = false;
static bool g_buildHack = false;
static bool g_unlimitedItems = false;
static bool g_noFallDamage = false;
static float g_speedMult = 1.F;
static float g_jumpMult = 1.F;
static bool g_noCold = false;
static bool g_maxSanity = false;
static bool g_maxFuel = false;
static bool g_esp = false;
static bool g_ohk = false;
static bool g_doLight = false;
static bool g_fastTreeCut = false;
static DebugConsole* g_debugConsole = nullptr;
static Unity::GameObject* g_light = nullptr;

static void Hooked_LocalPlayer__Awake(LocalPlayer* __this);
static void Hooked_LocalPlayer__OnDestroy(LocalPlayer* __this);
static void Hooked_PlayerStats__Update(PlayerStats* __this);
static bool Hooked_PlayerStats__get_Warm(PlayerStats* __this);
static void Hooked_BatteryBasedLight__Update(void* __this);
static int  Hooked_InventoryItem__Add(InventoryItem* __this, int amount, bool isEquiped);
static void Hooked_FirstPersonCharacter__HandleLanded(FirstPersonCharacter* __this);
static void Hooked_FirstPersonCharacter__HandleWalkingSpeedOptions(FirstPersonCharacter* __this);
static void Hooked_FirstPersonCharacter__HandleRunningStaminaAndSpeed(FirstPersonCharacter* __this);
static float Hooked_FirstPersonCharacter__CalculateJumpVerticalSpeed(FirstPersonCharacter* __this);
static void Hooked_EnemyHealth__Hit(FirstPersonCharacter* __this, int damage);

struct Scene {
    static void* ActiveMB() { return *Scene__ActiveMB; }
    static Unity::GameObject* Yacht() { return *Scene__Yacht; }
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
        void* args[1] = { nullptr };
        DebugConsole___addClothingOutfitRandom.Invoke(this, args);
    }
    
    void _addAllStoryItems() { 
        void* args[1] = { nullptr };
        DebugConsole___addAllStoryItems.Invoke(this, args);
    }
    
    void _addAllItems() { 
        void* args[1] = { nullptr };
        DebugConsole___addAllItems.Invoke(this, args);
    }
};

struct LocalPlayer {
    static Unity::GameObject* GameObject() { return *LocalPlayer__GameObject; }
    static Unity::Transform* Transform() { return *LocalPlayer__Transform; }
    static PlayerStats* Stats() { return *LocalPlayer__Stats; }
};

struct mutantController {
    System::List<Unity::GameObject*>* activeCannibals() { return *(System::List<Unity::GameObject*>**)((char*)this + mutantController__activeCannibals); }
};

struct AccountInfo {
    static bool UnlockAchievement(AchievementData* ach) {
        void* args[1] = { ach };
        return AccountInfo__UnlockAchievement.Invoke(nullptr, args);
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
};

struct SpawnPoolsDict {
    SpawnPool* get_Item(System::String* index) {
        void* args[1] = { index };
        return (SpawnPool*)SpawnPoolsDict__get_Item.Invoke(this, args);
    }
};

struct PlayerStats {
    float& Fullness() { return *(float*)((char*)this + PlayerStats__Fullness); }
    float& Thirst() { return *(float*)((char*)this + PlayerStats__Thirst); }
    float& BatteryCharge() { return *(float*)((char*)this + PlayerStats__BatteryCharge); }
};

struct InventoryItem {
    int& _amount() { return *(int*)((char*)this + InventoryItem___amount); }
};

struct FirstPersonCharacter {
    bool& allowFallDamage() { return *(bool*)((char*)this + FirstPersonCharacter__allowFallDamage); }
    float& speed() { return *(float*)((char*)this + FirstPersonCharacter__speed); }
public:
    void Die() { EnemyHealth__Die.Invoke(this, nullptr); }
};

static void Enable() {
    g_debugConsole = (DebugConsole*)DebugConsole::NewI(Class::Find("Assembly-CSharp", "TheForest", "DebugConsole"));
    g_light = Unity::GameObject::ctor("PlayerLight98");
    g_light->get_transform()->set_parent(LocalPlayer::Transform());
    Unity::Vector3 newPos = LocalPlayer::Transform()->get_position();
    newPos.y += 2.F;
    g_light->get_transform()->set_position(newPos);
    g_light->SetActive(false);
    Unity::Light* light = (Unity::Light*)g_light->AddComponent(Class::Find("UnityEngine", "UnityEngine", "Light")->Type());
    light->set_intensity(.5f);
    light->set_range(1000.f);
    g_camera = Unity::Camera::main();
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
    DetourAttach(&(PVOID&)(LocalPlayer__Awake.Ptr()), Hooked_LocalPlayer__Awake);
    DetourAttach(&(PVOID&)(LocalPlayer__OnDestroy.Ptr()), Hooked_LocalPlayer__OnDestroy);
    DetourTransactionCommit();

    if (Scene::ActiveMB())
        Enable();
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

void HaxSdk::RenderBackground() {
    if (g_inGame && g_esp) {
        SpawnPoolsDict* dict = *PoolManager__Pools;
        System::List<Unity::Transform*>* spawned = dict->get_Item(System::String::New("creatures"))->_spawned();
        for (int32_t i = 0; i < spawned->Lenght(); ++i) {
            Unity::GameObject* go = spawned->Data()[i]->get_gameObject();
            if (go) {
                Unity::Vector3 creaturePos = go->get_transform()->get_position();
                Unity::Vector3 playerPos = LocalPlayer::Transform()->get_position();
                float dist = Unity::Vector3::Distance(playerPos, creaturePos);
                Unity::Vector3 screenPos = g_camera->WorldToScreenPoint(creaturePos);
                if (screenPos.z > 0) {
                    char* name = go->get_name()->UTF8();
                    //ImGui::GetBackgroundDrawList()->AddLine(ImVec2(globals::g_screenWidth / 2.f, globals::g_screenHeight), ImVec2(screenPos.x, globals::g_screenHeight - screenPos.y), 0xFF00FF00, 1.5f);
                    AddText(name, ImVec2(screenPos.x, globals::g_screenHeight - screenPos.y - 20.f), 0xFF00FF00);
                }
            }
        }
    }
}

void HaxSdk::RenderMenu() {
    ImGui::Begin("Menu");
    if (g_inGame) {
        ImGui::Checkbox("Godmode", Cheats__GodMode);
        ImGui::Checkbox("Infinite energy", Cheats__InfiniteEnergy);
        ImGui::Checkbox("Infinite battery", &g_infiniteBattery);
        ImGui::Checkbox("Max fullness", &g_maxFullness);
        ImGui::Checkbox("No thirst", &g_noThirst);
        ImGui::Checkbox("Free building", Cheats__Creative);
        ImGui::Checkbox("Unlimited items", &g_unlimitedItems);
        ImGui::SliderFloat("Jump amplification", &g_jumpMult, 1.F, 15.F, "%.1f");
        ImGui::Checkbox("No fall damage", &g_noFallDamage);
        ImGui::SliderFloat("Movement acceleration", &g_speedMult, 1.F, 5.F, "%.1f");
        ImGui::Checkbox("No fall damage", &g_noCold);
        ImGui::Checkbox("Max sanity", &g_maxSanity);
        if (ImGui::Button("Random outfit")) {
            static Class* debugConsoleClass = Class::Find("Assembly-CSharp", "TheForest", "DebugConsole");
            ((DebugConsole*)DebugConsole::NewI(debugConsoleClass))->_addClothingOutfitRandom();
        }
        if (ImGui::Button("Add all story items"))
            g_debugConsole->_addAllStoryItems();
        if (ImGui::Button("Add all items"))
            g_debugConsole->_addAllItems();
        ImGui::Checkbox("Infinite fuel", &g_maxFuel);
        ImGui::Checkbox("Unlimited hairspray", Cheats__UnlimitedHairspray);
        ImGui::Checkbox("One hit kills", &g_ohk);
        bool tmp = g_doLight;
        ImGui::Checkbox("Light", &tmp);
        if (tmp != g_doLight) {
            g_doLight = tmp;
            g_light->SetActive(tmp);
        }
        if (ImGui::Button("Unlock all achievements")) {
            System::Array<AchievementData*>* achievements = *Achievements__Data;
            for (int i = 0; i < achievements->Length(); ++i)
                AccountInfo::UnlockAchievement(achievements->Data()[i]);
        }
        ImGui::Checkbox("Tree cut", &g_fastTreeCut);
        if (ImGui::Button("None")) (*Scene__WeatherSystem)->TurnOn(WeatherSystem::None);
        ImGui::SameLine();
        if (ImGui::Button("Light")) (*Scene__WeatherSystem)->TurnOn(WeatherSystem::Light);
        ImGui::SameLine();
        if (ImGui::Button("Medium")) (*Scene__WeatherSystem)->TurnOn(WeatherSystem::Medium);
        ImGui::SameLine();
        if (ImGui::Button("Heavy")) (*Scene__WeatherSystem)->TurnOn(WeatherSystem::Heavy);
        ImGui::Checkbox("ESP", &g_esp);
    }
    ImGui::End();
}

static void Hooked_LocalPlayer__Awake(LocalPlayer* __this) {
    reinterpret_cast<void(*)(LocalPlayer*)>(LocalPlayer__Awake.Ptr())(__this);
    Enable();
}

static void Hooked_LocalPlayer__OnDestroy(LocalPlayer* __this) {
    g_inGame = false;
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

static int Hooked_InventoryItem__Add(InventoryItem* __this, int amount, bool isEquiped) {
    if (g_unlimitedItems) {
        __this->_amount() += amount;
        return 0;
    }
    return reinterpret_cast<int(*)(InventoryItem*,int,bool)>(InventoryItem__Add.Ptr())(__this, amount, isEquiped);
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

static void Hooked_EnemyHealth__Hit(FirstPersonCharacter* __this, int damage) {
    if (g_ohk) {
        __this->Die();
        return;
    }
    reinterpret_cast<void(*)(FirstPersonCharacter*,int)>(EnemyHealth__Hit.Ptr())(__this, damage);
}

static ImVec4 HexToColor(std::string hex_string) {
    int r, g, b, a;
    sscanf_s(hex_string.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
    ImVec4 color{ (float(r) / 255), (float(g) / 255), (float(b) / 255), (float(a) / 255) };
    return color;
}

void HaxSdk::ApplyStyle() {
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    float fontSize = std::round(13 * (float)desktop.bottom / 1080);
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\trebucbd.ttf", fontSize);
    g_espFont = io.Fonts->AddFontFromMemoryTTF(g_fRubik, sizeof(g_fRubik), 32.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGuiStyle* styles = &ImGui::GetStyle();

    auto colors = styles->Colors;
    colors[ImGuiCol_Border] = HexToColor("26383FFF");
    colors[ImGuiCol_BorderShadow] = HexToColor("33333300");
    colors[ImGuiCol_Button] = HexToColor("23303DFF");
    colors[ImGuiCol_ButtonActive] = HexToColor("474968FF");
    colors[ImGuiCol_ButtonHovered] = HexToColor("444C70FF");
    colors[ImGuiCol_CheckMark] = HexToColor("A5BCDBFF");
    colors[ImGuiCol_ChildBg] = HexToColor("1E262BFF");
    colors[ImGuiCol_DragDropTarget] = HexToColor("FFFF00E5");
    colors[ImGuiCol_FrameBg] = HexToColor("2D3F44FF");
    colors[ImGuiCol_FrameBgActive] = HexToColor("30383DFF");
    colors[ImGuiCol_FrameBgHovered] = HexToColor("26303DFF");
    colors[ImGuiCol_Header] = HexToColor("0000003D");
    colors[ImGuiCol_HeaderActive] = HexToColor("0070EAFF");
    colors[ImGuiCol_HeaderHovered] = HexToColor("1E2833CC");
    colors[ImGuiCol_MenuBarBg] = HexToColor("1E232DFF");
    colors[ImGuiCol_ModalWindowDimBg] = HexToColor("CCCCCC59");
    colors[ImGuiCol_NavHighlight] = HexToColor("4296F9FF");
    colors[ImGuiCol_NavWindowingDimBg] = HexToColor("CCCCCC33");
    colors[ImGuiCol_NavWindowingHighlight] = HexToColor("FFFFFFB2");
    colors[ImGuiCol_PlotHistogram] = HexToColor("E5B200FF");
    colors[ImGuiCol_PlotHistogramHovered] = HexToColor("FF9900FF");
    colors[ImGuiCol_PlotLines] = HexToColor("9B9B9BFF");
    colors[ImGuiCol_PlotLinesHovered] = HexToColor("FF6D59FF");
    colors[ImGuiCol_PopupBg] = HexToColor("14161CEF");
    colors[ImGuiCol_ResizeGrip] = HexToColor("A3C9F93F");
    colors[ImGuiCol_ResizeGripActive] = HexToColor("6D8CB2F2");
    colors[ImGuiCol_ResizeGripHovered] = HexToColor("A5BFDDAA");
    colors[ImGuiCol_ScrollbarBg] = HexToColor("1C1C1C63");
    colors[ImGuiCol_ScrollbarGrab] = HexToColor("875E5EFF");
    colors[ImGuiCol_ScrollbarGrabActive] = HexToColor("8E1919FF");
    colors[ImGuiCol_ScrollbarGrabHovered] = HexToColor("7C3A3AFF");
    colors[ImGuiCol_Separator] = HexToColor("333F49FF");
    colors[ImGuiCol_SeparatorActive] = HexToColor("6B91BFFF");
    colors[ImGuiCol_SeparatorHovered] = HexToColor("4F7299C6");
    colors[ImGuiCol_SliderGrab] = HexToColor("5977ADFF");
    colors[ImGuiCol_SliderGrabActive] = HexToColor("ADCCFFFF");
    colors[ImGuiCol_Tab] = HexToColor("1C262BFF");
    colors[ImGuiCol_TabActive] = HexToColor("333F49FF");
    colors[ImGuiCol_TabHovered] = HexToColor("969696CC");
    colors[ImGuiCol_TabUnfocused] = HexToColor("1C262BFF");
    colors[ImGuiCol_TabUnfocusedActive] = HexToColor("1C262BFF");
    colors[ImGuiCol_TableBorderLight] = HexToColor("3A3A3FFF");
    colors[ImGuiCol_TableBorderStrong] = HexToColor("4F4F59FF");
    colors[ImGuiCol_TableHeaderBg] = HexToColor("303033FF");
    colors[ImGuiCol_TableRowBg] = HexToColor("333F49FF");
    colors[ImGuiCol_TableRowBgAlt] = HexToColor("1C262BFF");
    colors[ImGuiCol_Text] = HexToColor("F2F4F9FF");
    colors[ImGuiCol_TextDisabled] = HexToColor("2B353DFF");
    colors[ImGuiCol_TextSelectedBg] = HexToColor("4296F959");
    colors[ImGuiCol_TitleBg] = HexToColor("232D38A5");
    colors[ImGuiCol_TitleBgActive] = HexToColor("212830FF");
    colors[ImGuiCol_TitleBgCollapsed] = HexToColor("26262682");
    colors[ImGuiCol_WindowBg] = HexToColor("1E2623FF");

    //Styles
    styles->Alpha = 1.0;
    styles->AntiAliasedFill = true;
    styles->AntiAliasedLines = true;
    styles->AntiAliasedLinesUseTex = true;
    styles->ButtonTextAlign = ImVec2(0.5, 0.5);
    styles->CellPadding = ImVec2(4.0, 2.0);
    styles->ChildBorderSize = 1.0;
    styles->ChildRounding = 5.0;
    styles->CircleTessellationMaxError = 0.30000001192092896;
    styles->ColorButtonPosition = ImGuiDir_Right;
    styles->ColumnsMinSpacing = 6.0;
    styles->CurveTessellationTol = 1.25;
    styles->DisabledAlpha = 0.6000000238418579;
    styles->DisplaySafeAreaPadding = ImVec2(3.0, 3.0);
    styles->DisplayWindowPadding = ImVec2(19.0, 19.0);
    styles->FrameBorderSize = 0.0;
    styles->FramePadding = ImVec2(4.0, 3.0);
    styles->FrameRounding = 4.0;
    styles->GrabMinSize = 10.0;
    styles->GrabRounding = 4.0;
    styles->IndentSpacing = 21.0;
    styles->ItemInnerSpacing = ImVec2(4.0, 4.0);
    styles->ItemSpacing = ImVec2(8.0, 4.0);
    styles->LogSliderDeadzone = 4.0;
    styles->MouseCursorScale = 1.0;
    styles->PopupBorderSize = 1.0;
    styles->PopupRounding = 0.0;
    styles->ScrollbarRounding = 9.0;
    styles->ScrollbarSize = 14.0;
    styles->SelectableTextAlign = ImVec2(0.0, 0.0);
    styles->TabBorderSize = 0.0;
    styles->TabMinWidthForCloseButton = 0.0;
    styles->TabRounding = 4.0;
    styles->TouchExtraPadding = ImVec2(0.0, 0.0);
    styles->WindowBorderSize = 1.0;
    styles->WindowMenuButtonPosition = ImGuiDir_Left;
    styles->WindowMinSize = ImVec2(32.0, 32.0);
    styles->WindowPadding = ImVec2(8.0, 8.0);
    styles->WindowRounding = 0.0;
    styles->WindowTitleAlign = ImVec2(0.0, 0.5);
}