#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Dobby/dobby.h"
#include "Data/Fonts/Roboto-Regular.h"
#include "Includes/Macros.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"
#include "ImGui/Font/pixel.h"
#include "ImGui/Font/SkeetFont.h"

#define targetLibName OBFUSCATE("libil2cpp.so")

#include "Menu/Themes.h"
//#include "ESP/include.h"
#include "Canvas/ESP.h"

#include "Chams/Chams.h"
#include "Includes/monoString.h"
#include "oxorany/oxorany_include.h"
#include "ImGui/Font/verdana.h"
#include "ByNameModding/BNM.hpp"
#include "Includes/fake_dlfcn.h"

using namespace std;
using namespace BNM;
using namespace ImGui;

int glHeight, glWidth;
bool setup;
uintptr_t address;



vector<void*> players;

void clearPlayers() {
    vector<void*> pls;
    for (int i = 0; i < players.size(); i++) {
        if (players[i] != NULL) {
            pls.push_back(players[i]);
        }
    }
    players = pls;
}

bool playerFind(void *pl) {
    if (pl != NULL) {
        for (int i = 0; i < players.size(); i++) {
            if (pl == players[i]) return true;
        }
    }
    return false;
}


#define HOOKAF(ret, func, ...) \
    ret (*orig##func)(__VA_ARGS__); \
    ret my##func(__VA_ARGS__)

HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

bool menuOpened = true;
float alpha = 0.0f;

bool p1 = true;
bool p2,p3,p4,p5, p6;

ImFont* SkeetNormal;
ImFont* Pixel;
ImFont* Verdana123;

static void *glHandle = nullptr;

struct My_Patches {
    MemoryPatch Sh, antiban1, antiban2, antiban3,
    antiban4, antiban5, antiban6;
} hexPatches;

bool Esp, ESPBox, ESPline;
ImVec4 linecolor = ImVec4(1, 1, 1, 1);
ImVec4 boxcolor = ImVec4(1, 1, 1, 1);
int linetype = 0;

void* atu;
Vector3 flypos;
bool noclip;
float noclipSpeed = 10;
int xu = 0;
int yu = 0;

/*monoString *(*il2cpp_string_new1)(const char *str);

void initDlsymHooks(const char *name) {
    void *handle = dlopen(name, 0);
    while (!handle) {
        handle = dlopen(name, 0);
        sleep(1);
    }
    
    il2cpp_string_new1 = (monoString *(*)(const char *)) dlsym(handle, oxorany("il2cpp_string_new1"));
}*/

void *(*get_Transform)(void *instance);
Vector3 (*get_position)(void *instance);
void *(*get_camera)();
Vector3 (*get_screenpos)(void *cam, Vector3 pos);
Vector3 (*get_right)(void* transform);
Vector3 (*getAxis)(void*);
float (*get_deltaTime)();
Vector3(*get_forward) (void*);
void (*set_position)(void* transform, Vector3 newPosition);

Vector3 getPosition(void *transform) {
    return get_position(get_Transform(transform));
} 

Vector3 (*WorldToScreenPoint)(void *instance, Vector3);

bool *IsLocal(void *player) {
	return *(bool **)((uint64_t) player + 0x10); 
}

void *get_photon(void *player) {
	return *(void **)((uint64_t) player + 0x44); 
}
 
int (*Screen_get_height)();
int (*Screen_get_width)();

bool spoofnickname;

bool godMode = false;
int chamsMode = 0;
bool speedHack = false;
float speedMultiplier = 2.0f;
bool spamChat = true;
int spamDelay = 100;
bool isHost = false;
bool freezeGranny = false;
bool spinGranny = false;
float spinSpeed = 500.0f;
bool antiTrap = false;
bool alwaysSprint = false;
bool noCrouch = false;
bool giveHammer = false;
bool giveSafeKey = false;
bool giveHouseKey = false;
bool giveCrossbow = false;
bool giveArrow = false;
bool giveShotgun = false;
bool giveCarKey = false;
bool giveCarBattery = false;
bool giveGasCan = false;
bool giveWrench = false;
bool giveSparkPlug = false;
bool givePepperSpray = false;
bool giveChainCutter = false;
bool giveTeddy = false;
bool get_host;
bool autohost = true;
bool close_connection, destroy_enemy, destroy_granny, destroy_dedus;

bool scale_players, tpUp, underground, insky;
bool get_owner;
bool kill_granny, invisible_granny, doors_granny, attack_granny, godmode_granny, scale_granny;
bool freezeDedus, kill_dedus, invisible_dedus, doors_dedus, attack_dedus, godmode_dedus, scale_dedus;
float attackGrannyValue = 1.f;
float attackDedusValue = 1.f;
bool spinDedus;
float spinDedusSpeed;
int scalededusX = 1;
int scaleGrannyX = 1;
int scalePlayersX = 1.5;
bool noExplosion, noBeartrap, enemy_doors, antiMonster, allowedInput, offDoorsCollision;

bool (*PhotonNetwork_get_IsMasterClient)();
void* (*PhotonNetwork_get_LocalPlayer)();
bool (*PhotonNetwork_SetMasterClient)(void* player);
void (*DestroyPlayerObjects)(void *photonplayer);
void (*Destroy)(void *photon);
void (*Revive)(void *photonplayer);
void (*set_aspect)(void* player, float value);
bool (*CloseConnection)(void *photonplayer);      
void* (*il2cpp_string_new)(const char* str);
void (*TMP_InputField_set_text)(void* instance);
void (*Transform_Rotate)(void* transform, float x, float y, float z);
void (*set_localScale)(void *transform, Vector3 newScale);

void *localPlayer = nullptr;
void* myPlayer = nullptr;
void *chatInstance = nullptr;
void *grannyInstance = nullptr;
void *dedusInstance = nullptr;
float originalWalkSpeed = 0.0f;
float originalSprintSpeed = 0.0f;
bool speedsStored = false;

#include <chrono>
auto lastSpamChat = std::chrono::steady_clock::now();

#include <string>

void AddColorPicker(const char* name, ImVec4 &color, bool prd = false, bool* rainbow = nullptr, bool* pulse = nullptr, bool* dark = nullptr) {
    ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_AlphaPreview;
    static ImVec4 backup_color;
    bool open_popup = ImGui::ColorButton((std::string(name) + std::string(oxorany("##3b"))).c_str(), color, misc_flags);
    if (open_popup) {
        ImGui::OpenPopup(name);
        backup_color = color;
    }
    if (ImGui::BeginPopup(name)) {
        ImGui::Spacing();
        ImGui::Text("%s", std::string(name).c_str());
        ImGui::Separator();
        ImGui::ColorPicker4("##picker", (float *) &color,misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaBar);
        ImGui::SameLine();
        ImGui::BeginGroup();
        ImGui::Text("%s",std::string("Current").c_str());
        ImGui::ColorButton("##current", color,ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf,ImVec2(60, 40));
        ImGui::Text("%s",std::string("Previous").c_str());
        if (ImGui::ColorButton("##previous", backup_color,ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf,ImVec2(60, 40)))color = backup_color;
        ImGui::EndGroup();
        if (prd) {
            if (rainbow) ImGui::Checkbox("rainbow", rainbow);
            if (pulse) ImGui::Checkbox("pulse", pulse);
            if (dark) ImGui::Checkbox("dark", dark);
        }
        ImGui::Spacing();
        ImGui::EndPopup();
    }
}
    
void ImGui::ShowExampleGameWindows(bool* p_open){
		
	ImGuiIO& io = ImGui::GetIO();
    
    const float buttonWidth = 750.0f;
    const float buttonHeight = 9.5f;
    const float bottomMargin = 23.0f;
    
    const ImVec2 min(ImVec2((glWidth / 2) - (buttonWidth / 2), 
                           glHeight - buttonHeight - bottomMargin));
    const ImVec2 max(ImVec2((glWidth / 2) + (buttonWidth / 2), 
                           glHeight - bottomMargin));
    
    ImGui::SetNextWindowPos(min);
    ImGui::SetNextWindowSize(ImVec2(buttonWidth, buttonHeight + 45));
    
    static auto swipebar_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | 
                                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | 
                                 ImGuiWindowFlags_NoScrollbar | //ImGuiWindowFlags_AlwaysAutoResize | 
                                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;
                                 
            

    if (ImGui::Begin(oxorany("SwipeBar"), nullptr, swipebar_flags)) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.23f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 1.0f, 1.0f, 0.23f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 0.23f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    
        if (ImGui::Button(oxorany("##SwipeBarButton"), ImVec2(buttonWidth, buttonHeight))) {
       menuOpened = !menuOpened;
        }
        
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);
        ImGui::End();
    }

	if (menuOpened){
    ImGuiIO &io = ImGui::GetIO();
		
		ImGuiStyle& style = ImGui::GetStyle();
	
	style.WindowRounding = 1.5f;
    style.FrameRounding = 8.0f;
    style.TabRounding = 8.0f;
    style.GrabRounding = 5.0f;
    style.ScrollbarRounding = 10.0f;
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(12, 8);
    style.ItemSpacing = ImVec2(12, 10);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 25.0f;
    style.GrabMinSize = 14.0f;
    
    // Убираем outline'ы для всех элементов
    style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Настройки цветов
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);
	
	style.ScrollbarRounding = 0.f;
    style.GrabMinSize = 0;

    style.Colors[ImGuiCol_WindowBg]          = ImColor(20, 20, 20, 255);
    style.Colors[ImGuiCol_ChildBg]           = ImColor(16, 16, 16, 255);
    style.Colors[ImGuiCol_FrameBg]           = style.Colors[ImGuiCol_WindowBg];
    style.Colors[ImGuiCol_Border]            = ImColor(0, 0, 0, 255);
    style.Colors[ImGuiCol_Header]            = ImColor(255, 255, 255, 0);
    style.Colors[ImGuiCol_HeaderHovered]     = ImColor(255, 255, 255, 0);
    style.Colors[ImGuiCol_HeaderActive]      = ImColor(255, 255, 255, 0);
    style.Colors[ImGuiCol_Button]            = style.Colors[ImGuiCol_WindowBg];
    style.Colors[ImGuiCol_ButtonHovered]     = ImColor(16, 16, 16, 255);
    style.Colors[ImGuiCol_ButtonActive]      = ImColor(14, 14, 14, 255);
    style.Colors[ImGuiCol_PopupBg]           = style.Colors[ImGuiCol_WindowBg];
    style.Colors[ImGuiCol_TextSelectedBg]    = ImColor(255, 255, 255, 20);
    style.Colors[ImGuiCol_ResizeGrip]        = ImColor(40, 40, 40, 255);
    style.Colors[ImGuiCol_ResizeGripActive]  = ImColor(40, 40, 40, 255);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(40, 40, 40, 255);
	
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowSize(ImVec2(1000, 690), ImGuiCond_Once);
        if (ImGui::Begin(oxorany("##dss"), p_open,
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoTitleBar)) {
			
			ImGui::PushFont(Verdana123);
			
		ImGui::BeginChild(oxorany("Header"), ImVec2(0, 70), true);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25);
        float textWidth = ImGui::CalcTextSize(oxorany("sn0w")).x;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - textWidth) * 0.5f);
        ImGui::Text(oxorany("by David"));
        ImGui::EndChild();
		
		for (int i = 0; i < 8; i++) {
		
		ImColor borderCol = ImColor(0, 0, 0, 255);

            if (i == 1 || i == 8 - 1)
                borderCol = ImColor(55, 55, 55, 255);
            else if (i == 0)
                borderCol = ImColor(0, 0, 0, 255);
            else
                borderCol = ImColor(35, 35, 35, 255);

            ImGui::GetWindowDrawList()->AddRect(ImVec2(ImGui::GetWindowPos().x + i, ImGui::GetWindowPos().y + i), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - i, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y - i), borderCol);
	}
		
	if(ImGui::Button(oxorany("visuals"),ImVec2(150,50))) {
    p1 = true;
	p2 = false;
    p3 = false;
    p4 = false;
	p5 = false;
	p6 = false;
	}if(ImGui::Button(oxorany("players"),ImVec2(150,50))) {
	p1 = false;
	p2 = true;
    p3 = false;
    p4 = false;
	p5 = false;
	p6 = false;
	} if(ImGui::Button(oxorany("granny"),ImVec2(150,50))) {
    p1 = false;
	p2 = false;
    p3 = true;
    p4 = false;
	p5 = false;
	p6 = false;
	} if(ImGui::Button(oxorany("grandpa"),ImVec2(150,50))) {
	p1 = false;
	p2 = false;
    p3 = false;
    p4 = true;
	p5 = false;
	p6 = false;
	} if(ImGui::Button(oxorany("game"),ImVec2(150,50))) {
	p1 = false;
	p2 = false;
    p3 = false;
    p4 = false;
	p5 = true;
	p6 = false;
	} if(ImGui::Button(oxorany("menu"),ImVec2(150,50))) {
	p1 = false;
	p2 = false;
    p3 = false;
    p4 = false;
	p5 = false;
	p6 = true;
	} 
        if (p1) {
	ImGui::SetCursorPos({165,90});
	ImGui::BeginChild(oxorany("##child1"),ImVec2(355,500),true);
	ImGui::SliderInt(oxorany("chams"), &chamsMode1, 0, 3);
	ImGui::EndChild();
	
ImGui::SameLine();
ImGui::BeginChild(oxorany("child2"), ImVec2(355,500));
ImGui::SetCursorPos(ImVec2(6,3));
    ImGui::SliderFloat(oxorany("red"), &r, 0.f, 255.f);
	ImGui::SliderFloat(oxorany("green"), &g, 0.f, 255.f);
	ImGui::SliderFloat(oxorany("blue"), &b, 0.f, 255.f);
	ImGui::EndChild();
}
if (p2) {
	ImGui::SetCursorPos({165,90});
	ImGui::BeginChild(oxorany("right##child"),ImVec2(305,500),true);
	if (ImGui::Button(oxorany("kick all"))) {
		close_connection = true;
	}
	ImGui::Checkbox(oxorany("scale"), &scale_players);
	    if (scale_players) {
		ImGui::SliderInt(oxorany("scale value"), &scalePlayersX, 1, 10);
	}
	
	ImGui::EndChild();
	
	ImGui::SameLine();
ImGui::BeginChild(oxorany("child2"), ImVec2(305,500));
ImGui::SetCursorPos(ImVec2(6,3));
	ImGui::Checkbox(oxorany("mini mode"), &tpUp);
	    if (tpUp) {
	scale_players = true;
	if (scale_players) {
	scalePlayersX = 0;
	}
}
ImGui::EndChild();
	}
if (p3) {
	ImGui::SetCursorPos({165,90});
	ImGui::BeginChild(oxorany("right##child"),ImVec2(305,500),true);
	ImGui::SameLine();
		if (ImGui::Button(oxorany("kill"))) {
		kill_granny = true;
	}
	ImGui::SameLine();
	if (ImGui::Button(oxorany("destroy"))) {
		destroy_granny = true;
	}
	    ImGui::Checkbox(oxorany("freeze"), &freezeGranny);
	ImGui::Checkbox(oxorany("scale"), &scale_granny);
	    if (scale_granny) {
		ImGui::SliderInt(oxorany("scale value"), &scaleGrannyX, 0, 10);
	}
	
	ImGui::EndChild();
	
	ImGui::SameLine();
ImGui::BeginChild(oxorany("child2"), ImVec2(305,500));
ImGui::SetCursorPos(ImVec2(6,3));
	ImGui::Checkbox(oxorany("godmode"), &godmode_granny);
		ImGui::Checkbox(oxorany("cant see players"), &invisible_granny);
		ImGui::Checkbox(oxorany("attack range"), &attack_granny);
	    if (attack_granny) {
		ImGui::SliderFloat(oxorany("range value"), &attackGrannyValue, 0.f, 100.0f);
	}
ImGui::EndChild();
	}
if (p4) {
	ImGui::SetCursorPos({165,90});
	ImGui::BeginChild(oxorany("right##child"),ImVec2(305,500),true);
		
	ImGui::SameLine();
		if (ImGui::Button(oxorany("kill"))) {
		kill_dedus = true;
	}
	ImGui::SameLine();
	if (ImGui::Button(oxorany("destroy"))) {
		destroy_dedus = true;
	}
	ImGui::Checkbox(oxorany("freeze"), &freezeDedus);
	    ImGui::Checkbox(oxorany("scale"), &scale_dedus);
	    if (scale_dedus) {
		ImGui::SliderInt(oxorany("scale value"), &scalededusX, 0, 10);
	}
	
	ImGui::EndChild();
	
	ImGui::SameLine();
ImGui::BeginChild(oxorany("child2"), ImVec2(305,500));
ImGui::SetCursorPos(ImVec2(6,3));
	ImGui::Checkbox(oxorany("cant see players"), &invisible_dedus);
ImGui::EndChild();
	}
	if (p5) {
	ImGui::SetCursorPos({165,90});
	ImGui::BeginChild(oxorany("right##child"),ImVec2(355,500),true);
	
	ImGui::Checkbox(oxorany("Enemy cant open"), &enemy_doors);
	
	ImGui::EndChild();
	
	ImGui::SameLine();
ImGui::BeginChild(oxorany("child2"), ImVec2(355,500));
ImGui::SetCursorPos(ImVec2(6,3));
	ImGui::Checkbox(oxorany("Anti BearTrap"), &noBeartrap);
	ImGui::Checkbox(oxorany("disable fall"), &allowedInput);
	ImGui::Checkbox(oxorany("no door collision"), &offDoorsCollision);
	ImGui::Checkbox(oxorany("anti monster"), &antiMonster);
ImGui::EndChild();
	}
if (p6) {
	ImGui::SetCursorPos({165,90});
	ImGui::BeginChild(oxorany("right##child"),ImVec2(700,500),true);
	
	ImGui::Text(oxorany("telegram channel: t.me/harmonyware"));
	ImGui::Text(oxorany("made by pastacodewz @il2cppgetmethodoffset"));
	
ImGui::EndChild();
	}
    ImGui::End();
	}
  }
}



	
	
void SetupImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);
    ImGui_ImplOpenGL3_Init("#version 100");
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 48.0f;
    io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, 43, 43.0f);
    ImGui::GetStyle().ScaleAllSizes(6.0f);
	ImGui::GetStyle().WindowRounding = 25.0f;
    
    ImGuiStyle& style = ImGui::GetStyle();

	Pixel = io.Fonts->AddFontFromMemoryTTF(PixelFont, sizeof(PixelFont), 35, NULL);
	Verdana123 = io.Fonts->AddFontFromMemoryTTF(__verdana, sizeof(__verdana), 35, NULL);
    SkeetNormal = io.Fonts->AddFontFromMemoryTTF(&PixelFont, sizeof PixelFont, 21, NULL, io.Fonts->GetGlyphRangesCyrillic());
      //style.FrameBorderSize = 1.5f;
        
}

bool HandleInputEvent(JNIEnv*,int,int,int,int);

typedef enum{TOUCH_ACTION_DOWN=0,TOUCH_ACTION_UP,TOUCH_ACTION_MOVE}TOUCH_ACTION;

typedef struct
{
TOUCH_ACTION action;
float x;
float y;
int pointers;
float y_velocity;
float x_velocity;
}TOUCH_EVENT;

TOUCH_EVENT g_LastTouchEvent;

bool HandleInputEvent(JNIEnv*env,int motionEvent,int x,int y,int p)
{
float velocity_y=(float)((float)y-g_LastTouchEvent.y)/100.f;
g_LastTouchEvent={.action=(TOUCH_ACTION)motionEvent,.x=static_cast<float>(x),.y=static_cast<float>(y),.pointers=p,.y_velocity=velocity_y};
ImGuiIO&io=ImGui::GetIO();
io.MousePos.x=g_LastTouchEvent.x;
io.MousePos.y=g_LastTouchEvent.y;
if(motionEvent==2)
{
if(g_LastTouchEvent.pointers>1)
{
io.MouseWheel=g_LastTouchEvent.y_velocity;
io.MouseDown[0]=false;
}
else
io.MouseWheel=0;
}
if(motionEvent==0)
io.MouseDown[0]=true;
if(motionEvent==1)
io.MouseDown[0]=false;
return true;
}

bool(*old_nativeInjectEvent)(JNIEnv*,jobject,jobject);
bool hook_nativeInjectEvent(JNIEnv*env,jobject instance,jobject event)
{
jclass MotionEvent=env->FindClass(("android/view/MotionEvent"));
if(!env->IsInstanceOf(event,MotionEvent))
return old_nativeInjectEvent(env,instance,event);
jmethodID id_getAct=env->GetMethodID(MotionEvent,("getActionMasked"),("()I"));
jmethodID id_getX=env->GetMethodID(MotionEvent,("getX"),("()F"));
jmethodID id_getY=env->GetMethodID(MotionEvent,("getY"),("()F"));
jmethodID id_getPs=env->GetMethodID(MotionEvent,("getPointerCount"),("()I"));
HandleInputEvent(env,env->CallIntMethod(event,id_getAct),env->CallFloatMethod(event,id_getX),env->CallFloatMethod(event,id_getY),env->CallIntMethod(event,id_getPs));
if(!ImGui::GetIO().MouseDownOwnedUnlessPopupClose[0])
return old_nativeInjectEvent(env,instance,event);
return false;
}

jint(*old_RegisterNatives)(JNIEnv*,jclass,JNINativeMethod*,jint);
jint hook_RegisterNatives(JNIEnv*env,jclass destinationClass,JNINativeMethod*methods,jint totalMethodCount)
{
int currentNativeMethodNumeration;
for(currentNativeMethodNumeration=0;currentNativeMethodNumeration<totalMethodCount;++currentNativeMethodNumeration)
{
if(!strcmp(methods[currentNativeMethodNumeration].name,("nativeInjectEvent")))
DobbyHook(methods[currentNativeMethodNumeration].fnPtr,(void*)hook_nativeInjectEvent,(void**)&old_nativeInjectEvent);
}
return old_RegisterNatives(env,destinationClass,methods,totalMethodCount);
}

EGLBoolean(*old_eglSwapBuffers)(EGLDisplay,EGLSurface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy,EGLSurface surface)
{
eglQuerySurface(dpy,surface,EGL_WIDTH,&glWidth);
eglQuerySurface(dpy,surface,EGL_HEIGHT,&glHeight);
if(!setup)
{
SetupImgui();
setup=true;
}
ImGuiIO&io=ImGui::GetIO();
switch(g_LastTouchEvent.action)
{
case TOUCH_ACTION_MOVE:
if(g_LastTouchEvent.pointers>1)
{
io.MouseWheel=g_LastTouchEvent.y_velocity;
io.MouseDown[0]=false;
}
else
io.MouseWheel=0;
break;
case TOUCH_ACTION_DOWN:
io.MouseDown[0]=true;
break;
case TOUCH_ACTION_UP:
io.MouseDown[0]=false;
break;
default:
break;
}

ImGui_ImplOpenGL3_NewFrame();
ImGui::NewFrame();
	
    ImGui::ShowExampleGameWindows();
    ImGui::Render();
	
    ImGui::EndFrame();
ImGui::Render();
glViewport(0,0,(int)io.DisplaySize.x,(int)io.DisplaySize.y);
ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
return old_eglSwapBuffers(dpy,surface);
}

//#define OFFSET_chat 0x90
#define OFFSET_sprintSpeed 0x280
#define OFFSET_walkSpeed 0x34
#define OFFSET_chat_inputField 0x20
#define OFFSET_chat_canSendMessage 0x44
#define OFFSET_granny_myTransform 0x78
#define OFFSET_granny_freeze 0x2FE

void (*orig_feedback_sendMessage)(void* instance);

void (*orig_playerController_Update)(void* instance);
void playerController_Update(void* instance) {
    if (instance) {
        localPlayer = instance;
     //   chatInstance = *(void**)((uintptr_t)instance + OFFSET_chat);
	
        if (PhotonNetwork_get_IsMasterClient) {
            isHost = PhotonNetwork_get_IsMasterClient();
        }
		
		if (autohost) {
		if (PhotonNetwork_SetMasterClient && PhotonNetwork_get_LocalPlayer) {
			void* localPlayer = PhotonNetwork_get_LocalPlayer();
			if (localPlayer) {
				PhotonNetwork_SetMasterClient(localPlayer);
			}
		}
	}
		
		if (close_connection) {
		CloseConnection(get_photon(instance));
	 }
        
        if (!speedsStored) {
            originalWalkSpeed = *(float*)((uintptr_t)instance + OFFSET_walkSpeed);
            originalSprintSpeed = *(float*)((uintptr_t)instance + OFFSET_sprintSpeed);
            if (originalWalkSpeed > 0) speedsStored = true;
        }
        
        if (speedHack && speedsStored) {
            *(float*)((uintptr_t)instance + OFFSET_walkSpeed) = originalWalkSpeed * speedMultiplier;
            *(float*)((uintptr_t)instance + OFFSET_sprintSpeed) = originalSprintSpeed * speedMultiplier;
        } else if (speedsStored) {
            *(float*)((uintptr_t)instance + OFFSET_walkSpeed) = originalWalkSpeed;
            *(float*)((uintptr_t)instance + OFFSET_sprintSpeed) = originalSprintSpeed;
        }
		
		if (scale_players) {
set_localScale(get_Transform(instance), Vector3(scalePlayersX, scalePlayersX, scalePlayersX));
} else {
set_localScale(get_Transform(instance), Vector3(1, 1, 1));
}
 
    }
    orig_playerController_Update(instance);
}

   #define OFFSET_kill_granny 0x1C6
   #define OFFSET_see_granny 0x110
   #define OFFSET_doors_granny 0x16C
     #define OFFSET_locked_doors_granny 0x16D
   #define OFFSET_attack_granny 0x124
   #define OFFSET_take_granny 0x145
   #define OFFSET_take2_granny 0x15F

void (*orig_AIGrannyController_FixedUpdate)(void* instance);
void AIGrannyController_FixedUpdate(void* instance) {
    if (instance) {
        grannyInstance = instance;
		
		if (destroy_granny) {
		Destroy(get_Transform(grannyInstance));
	 }
		
        if (isHost) {
          if (freezeGranny) {
                *(bool*)((uintptr_t)instance + OFFSET_granny_freeze) = true;
            }
		if (kill_granny) {
            *(bool*)((uintptr_t)instance + OFFSET_kill_granny) = true;
        }
		if (invisible_granny) {
            *(bool*)((uintptr_t)instance + OFFSET_see_granny) = false;
        }
		if (attack_granny) {
            *(float*)((uintptr_t)instance + OFFSET_attack_granny) = attackGrannyValue;
        }
		if (godmode_granny) {
            *(bool*)((uintptr_t)instance + OFFSET_take_granny) = false;
			*(bool*)((uintptr_t)instance + OFFSET_take2_granny) = true;
        }
		if (scale_granny) {
set_localScale(get_Transform(instance), Vector3(scaleGrannyX, scaleGrannyX, scaleGrannyX));
} else {
set_localScale(get_Transform(instance), Vector3(1, 1, 1));
}
            if (spinGranny && Transform_Rotate) {
                void* grannyTransform = *(void**)((uintptr_t)instance + OFFSET_granny_myTransform);
                if (grannyTransform) {
                    Transform_Rotate(grannyTransform, 0.0f, spinSpeed * 0.02f, 0.0f);
                }
            }
        }
		if (!playerFind(instance)) players.push_back(instance);
        if (players.size() > 15) {
            players.clear();
        }
    }
    orig_AIGrannyController_FixedUpdate(instance);
}

#define OFFSET_dedus_myTransform 0x44
#define OFFSET_dedus_freeze 0x34
   #define OFFSET_kill_dedus 0x215
   #define OFFSET_see_dedus 0x12C
   #define OFFSET_doors_dedus 0x194
   #define OFFSET_locked_doors_dedus 0x196
   #define OFFSET_attack_dedus 0x140
   #define OFFSET_take_dedus 0x48
   #define OFFSET_take2_dedus 0x183
   
   

void (*orig_AIGrandpaController_FixedUpdate)(void* instance);
void AIGrandpaController_FixedUpdate(void* instance) {
    if (instance) {
        dedusInstance = instance;
		
		if (destroy_dedus) {
		Destroy(get_Transform(dedusInstance));
	 }
		
        if (isHost) {
		if (kill_dedus) {
            *(bool*)((uintptr_t)instance + OFFSET_kill_dedus) = true;
        }
		
		if (invisible_dedus) {
            *(bool*)((uintptr_t)instance + OFFSET_see_dedus) = false;
        }
		
		if (freezeDedus) {
            *(float*)((uintptr_t)instance + OFFSET_walkSpeed) = 0.f;
		}
		
		if (scale_dedus) {
set_localScale(get_Transform(instance), Vector3(scalededusX, scalededusX, scalededusX));
} else {
set_localScale(get_Transform(instance), Vector3(1, 1, 1));
}
            if (spinDedus && Transform_Rotate) {
                void* dedusTransform = *(void**)((uintptr_t)instance + OFFSET_dedus_myTransform);
                if (dedusTransform) {
                    Transform_Rotate(dedusTransform, 0.0f, spinDedusSpeed * 0.02f, 0.0f);
                }
            }
        }
		if (!playerFind(instance)) players.push_back(instance);
        if (players.size() > 15) {
            players.clear();
        }
    }
    orig_AIGrandpaController_FixedUpdate(instance);
}

void (*old_GrenadeTrigger)(void* instance);
void GrenadeTrigger(void* instance) {
    if (instance != nullptr) {
        if (noExplosion) {
            return;
        }
    }
    return old_GrenadeTrigger(instance);
}

void (*old_GrannyInDoors)(void* instance);
void GrannyInDoors(void* instance) {
    if (instance != nullptr) {
        if (enemy_doors) {
            return;
        }
    }
    return old_GrannyInDoors(instance);
}

void (*old_BeartrapTrigger)(void* instance);
void BeartrapTrigger(void* instance) {
    if (instance != nullptr) {
        if (noBeartrap) {
            return;
        }
    }
    return old_BeartrapTrigger(instance);
}

void (*old_MonsterEat)(void* instance);
void MonsterEat(void* instance) {
    if (instance != nullptr) {
        if (antiMonster) {
            return;
        }
    }
    return old_MonsterEat(instance);
}

void (*old_FallFloor)(void* instance);
void FallFloor(void* instance) {
        if (allowedInput) {
            return;
        }
    return old_FallFloor(instance);
}

void (*old_DoorCollision)(void* instance);
void DoorCollision(void* instance) {
        if (offDoorsCollision) {
            return;
        }
    return old_DoorCollision(instance);
}

void *hack_thread(void *) {
    ProcMap il2cppMap;
    do {
        il2cppMap = KittyMemory::getLibraryMap(libName);
        sleep(1);
    } while (!isLibraryLoaded(libName));
    setShader("_Color");
    LogShaders();
    Wallhack();
	
	auto addr=(uintptr_t)dlsym(RTLD_NEXT,"eglSwapBuffers");
    DobbyHook((void*)addr,(void*)hook_eglSwapBuffers,(void**)&old_eglSwapBuffers);
    
    Screen_get_height = (int (*)())
                getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x30C167C));//Screen
    Screen_get_width = (int (*)()) 
                getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x30C1638));//Screen
    
                
    WorldToScreenPoint = (Vector3(*)(void*, Vector3)) 
                getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x30B6774));//Camera WorldToScreenPoint(Vector3 position)
    get_position = (Vector3 (*)(void*)) 
                getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x30FF354));//Transform get_position
    get_Transform = (void *(*)(void*)) 
                getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x30EC278));//Component get_transform
    get_camera = (void*(*)()) 
                getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x30B6B3C));//Camera get_main
	set_localScale = (void (*)(void*, Vector3)) 
	            getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x31000F0));//Transform set_localscale
	set_position = (void (*)(void*, Vector3)) 
	            getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x2EF56A8));//Transform set_position

    MSHookFunction((void *) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xE3B0FC)), (void *)playerController_Update, (void **) &orig_playerController_Update);
	//MSHookFunction((void *) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xDCFCD0)), (void *)GrenadeTrigger, (void **) &old_GrenadeTrigger);
	MSHookFunction((void *) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xE1F164)), (void *)GrannyInDoors, (void **) &old_GrannyInDoors);
	MSHookFunction((void *) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xDC3334)), (void *)BeartrapTrigger, (void **) &old_BeartrapTrigger);
	MSHookFunction((void *) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xE4BD50)), (void *)MonsterEat, (void **) &old_MonsterEat);
	MSHookFunction((void *) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xE4CB14)), (void *)FallFloor, (void **) &old_FallFloor);
	MSHookFunction((void *) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xE8DCF4)), (void *)DoorCollision, (void **) &old_DoorCollision);
    MSHookFunction((void *) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xDFAC0C)), (void *)AIGrannyController_FixedUpdate, (void **) &orig_AIGrannyController_FixedUpdate);
	MSHookFunction((void *) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xDDC780)), (void *)AIGrandpaController_FixedUpdate, (void **) &orig_AIGrandpaController_FixedUpdate);
    
    Transform_Rotate = (void(*)(void*, float, float, float)) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x3100C08));
    orig_feedback_sendMessage = (void(*)(void*)) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0xE0ED54));
    TMP_InputField_set_text = (void(*)(void*)) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x2FE9898));
    PhotonNetwork_get_IsMasterClient = (bool(*)()) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x262779C));
    PhotonNetwork_get_LocalPlayer = (void*(*)()) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x2629D44));
	PhotonNetwork_SetMasterClient = (bool(*)(void*)) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x262F3FC));
    CloseConnection = (bool (*)(void *)) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x2631FE4));
	Destroy = (void (*)(void *)) getAbsoluteAddress(oxorany("libil2cpp.so"), oxorany(0x2635610));
											
                                            
        /*
        MemoryPatch::createWithHex("libil2cpp.so", 
                0x821E28, 
               "FA 04 44 E3 1E FF 2F E1").Modify();
               0x674E10 0x66CC78
            
     01 00 A0 E3 1E FF 2F E1 = True or number 1
     02 00 A0 E3 1E FF 2F E1 = Number 2
     07 00 A0 E3 1E FF 2F E1 = Number 7
     0A 00 A0 E3 1E FF 2F E1 = Number 10
     0F 00 A0 E3 1E FF 2F E1 = Number 15
     10 00 A0 E3 1E FF 2F E1 = Number 16
     11 00 A0 E3 1E FF 2F E1 = Number 17
     01 00 A0 E3 1E FF 2F E1 = 1/True
     00 00 A0 E3 1E FF 2F E1 = 0/False
     FA 04 44 E3 1E FF 2F E1 = FLOAT/2000
     c6 02 44 e3 1e ff 2f e1 = FLOAT/99
     20 02 44 E3 1E FF 2F E1 = FLOAT/40
     C8 02 44 E3 1E FF 2F E1 = FLOAT/100
     C0 0F 43 E3 1E FF 2F E1 = FLOAT/1.5
     37 00 A0 E3 1E FF 2F E1 = 50 INT
     FF 00 A0 E3 1E FF 2F E1 = 255 INT
     E7 03 00 E3 1E FF 2F E1 = 999 INT
     DC 0F 0F E3 1E FF 2F E1 = 65500/INT
     FF 09 0C E3 00 10 A0 E3 9A 0B 43 E3 1E FF 2F E1 = 999,999,999 FLOAT
     */
     
    pthread_exit(nullptr);
    return nullptr;
}
extern"C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM*vm,void*reserved)
{
JNIEnv*globalEnv;
vm->GetEnv((void**)&globalEnv,JNI_VERSION_1_6);
DobbyHook((void*)globalEnv->functions->RegisterNatives,(void*)hook_RegisterNatives,(void**)&old_RegisterNatives);
pthread_t ptid;
pthread_create(&ptid,NULL,hack_thread,NULL);
return JNI_VERSION_1_6;
}
