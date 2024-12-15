#include "cheat.h"

#include "haxsdk/haxsdk.h"
#include "haxsdk/third_party/imgui/imgui.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct mutantController;
struct Scene;
struct Cheats;
struct DebugConsole;
struct AchievementData;
struct WeatherSystem;

#define HAXSDK_FUNCTION(a, n, c, m, s)          BackendMethod c ## __ ## m
#define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  BackendMethod c ## __ ## m
#define HAXSDK_STATIC_FIELD(a, n, c, f, t)      static t* c ## __ ## f
#define HAXSDK_FIELD_OFFSET(a, n, c, f)         static int c ## __ ## f
#include "cheat_data.h"

namespace globals {
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
    static GameObject* g_light = nullptr;
}

struct Scene {
    static void* ActiveMB() { return *Scene__ActiveMB; }
    static GameObject* Yacht() { return *Scene__Yacht; }
    static mutantController* MutantControler() { return *Scene__MutantControler; }
};

struct Cheats {
    static bool& Godmode() { return *Cheats__GodMode; }
    static bool& InfiniteEnergy() { return *Cheats__InfiniteEnergy; }
    static bool& Creative() { return *Cheats__Creative; }
    static bool& UnlimitedHairspray() { return *Cheats__UnlimitedHairspray; }
};

struct DebugConsole : Component {
    void _addClothingOutfitRandom() { 
        void* args[1] = { nullptr };
        DebugConsole___addClothingOutfitRandom.pBase->invoke(this, args);
    }
    
    void _addAllStoryItems() { 
        void* args[1] = { nullptr };
        DebugConsole___addAllStoryItems.pBase->invoke(this, args);
    }
    
    void _addAllItems() { 
        void* args[1] = { nullptr };
        DebugConsole___addAllItems.pBase->invoke(this, args);
    }
};

struct LocalPlayer {
    static GameObject* GameObject() { return *LocalPlayer__GameObject; }
    static Transform* Transform() { return *LocalPlayer__Transform; }
};

struct mutantController {
    List<GameObject*>* activeCannibals() { return *(List<GameObject*>**)((char*)this + mutantController__activeCannibals); }
};

struct AccountInfo {
    static bool UnlockAchievement(AchievementData* ach) {
        void* args[1] = { ach };
        return AccountInfo__UnlockAchievement.pBase->invoke(nullptr, args);
    }
};

struct WeatherSystem {
    enum RainTypes { None, Light, Medium, Heavy };
    void TurnOn(RainTypes type) {
        void* args[1] = {&type};
        WeatherSystem__TurnOn.pBase->invoke(this, args);
    }
};

void ModMenu::Initialize() {
    #define HAXSDK_FUNCTION(a, n, c, m, s)     c ## __ ## m = BackendClass::find(a, n, #c)->find_method(#m, s)
    #define HAXSDK_FUNCTION_EXT(a, n, c, m, mn, s)  c ## __ ## m = BackendClass::find(a, n, #c)->find_method(mn, s)
    #define HAXSDK_STATIC_FIELD(a, n, c, f, t) c ## __ ## f = (t*)BackendClass::find(a, n, #c)->find_static_field(#f)
    #define HAXSDK_FIELD_OFFSET(a, n, c, f)    c ## __ ## f = BackendClass::find(a, n, #c)->find_field(#f)->offset()
    #include "cheat_data.h"
}

void HaxSdk::RenderBackground() {

}

void HaxSdk::RenderMenu() {
    ImGui::Begin("Menu");
    if (Scene::ActiveMB()) {
        if (!globals::g_debugConsole)
            globals::g_debugConsole = (DebugConsole*)DebugConsole::alloc(BackendClass::find("Assembly-CSharp", "TheForest", "DebugConsole"));
        if (!globals::g_light) {
            globals::g_light = GameObject::ctor("PlayerLight98");
            globals::g_light->get_transform()->set_parent(LocalPlayer::Transform());
            Vector3 newPos = LocalPlayer::Transform()->get_position();
            newPos.y += 2.F;
            globals::g_light->get_transform()->set_position(newPos);
            globals::g_light->SetActive(false);
            Light* light = (Light*)globals::g_light->AddComponent(BackendClass::find("UnityEngine", "UnityEngine", "Light")->type()->system_type());
            light->set_intensity(.5f);
            light->set_range(1000.f);
        }
        ImGui::Checkbox("Godmode", Cheats__GodMode);
        ImGui::Checkbox("Infinite energy", Cheats__InfiniteEnergy);
        ImGui::Checkbox("Infinite battery", &globals::g_infiniteBattery);
        ImGui::Checkbox("Max fullness", &globals::g_maxFullness);
        ImGui::Checkbox("No thirst", &globals::g_noThirst);
        ImGui::Checkbox("Free building", Cheats__Creative);
        ImGui::Checkbox("Unlimited items", &globals::g_unlimitedItems);
        ImGui::SliderFloat("Jump amplification", &globals::g_jumpMult, 1.F, 15.F, "%.1f");
        ImGui::Checkbox("No fall damage", &globals::g_noFallDamage);
        ImGui::SliderFloat("Movement acceleration", &globals::g_speedMult, 1.F, 5.F, "%.1f");
        ImGui::Checkbox("No fall damage", &globals::g_noCold);
        ImGui::Checkbox("Max sanity", &globals::g_maxSanity);
        if (ImGui::Button("Random outfit") && globals::g_debugConsole)
            globals::g_debugConsole->_addClothingOutfitRandom();
        if (ImGui::Button("Add all story items"))
            globals::g_debugConsole->_addAllStoryItems();
        if (ImGui::Button("Add all items"))
            globals::g_debugConsole->_addAllItems();
        ImGui::Checkbox("Infinite fuel", &globals::g_maxFuel);
        ImGui::Checkbox("Unlimited hairspray", Cheats__UnlimitedHairspray);
        ImGui::Checkbox("One hit kills", &globals::g_ohk);
        bool tmp = globals::g_doLight;
        ImGui::Checkbox("Light", &tmp);
        if (tmp != globals::g_doLight) {
            globals::g_doLight = tmp;
            globals::g_light->SetActive(tmp);
        }
        if (ImGui::Button("Unlock all achievements")) {
            Array<AchievementData*>* achievements = *Achievements__Data;
            for (int i = 0; i < achievements->size(); ++i)
                AccountInfo::UnlockAchievement(achievements->data()[i]);
        }
        ImGui::Checkbox("Tree cut", &globals::g_fastTreeCut);
        if (ImGui::Button("None")) (*Scene__WeatherSystem)->TurnOn(WeatherSystem::None);
        ImGui::SameLine();
        if (ImGui::Button("Light")) (*Scene__WeatherSystem)->TurnOn(WeatherSystem::Light);
        ImGui::SameLine();
        if (ImGui::Button("Medium")) (*Scene__WeatherSystem)->TurnOn(WeatherSystem::Medium);
        ImGui::SameLine();
        if (ImGui::Button("Heavy")) (*Scene__WeatherSystem)->TurnOn(WeatherSystem::Heavy);
    }
    ImGui::End();
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
    //std::cout << "Resolution, font: " << desktop.right << 'x' << desktop.bottom << ' ' << fontSize;
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\trebucbd.ttf", fontSize);

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