#ifndef BOOLS_H
#define BOOLS_H

#include <jni.h>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/mman.h>
#include <android/log.h>

bool Esp = false;
bool EspLine = false;
bool EspBox = false;
bool EspName = false;
bool EspHealth = false;
bool EspDistance = false;
bool EspObject = false;
bool EspSkeleton = false;
bool ESPTeamminate = false;

static bool ESPLine = false;
static bool ESPBox = false;
static bool CrossF = false;
static bool enemytp;
static bool testt;
static bool freezepos;
static float isEnemyPlayerSizex, isEnemyPlayerSizey, isEnemyPlayerSizez;
static float ThicknessLine = 2.0f;
static float eposx;
static float eposy;
static float eposz;
static bool esetscale;
static bool tes1 = false;
static bool tes2 = false;
bool freezeg;
bool fov;
float fieldofview;

bool chams, wireframe, outline, glow, shading;
float redc, greenc, bluec, alphac = 255.f;

void* atu;
Vector3 flypos;
bool noclip;
float noclipSpeed = 20;
float xu = 0; float yu = 0;

bool testfunc = true;

bool godmode, startshoot, shotgunloaded, nocWalls;
bool autoswin;
bool grannycantsee, grannyarrow, grannycar, grannypapper, grannyfreeze, grannycantmove, grannyLongAttackRange, grannycantseedoors, infinitydays, grannyFastRestart, bighead;
bool haveAvbitare, haveHammare, havevas, havesafeKey, haveexitKey, havehanglockKey, havepadlockCode, havearmborst, haveArrow, haveweaponKey, havescrewdriver, haveplanka, havebattery, haveplayhouseKey, havecarKey, havemelon, haveteddy, haveoldShotgun, havecarbattery, havegascan, havewrench,
havesparkplug, havemeat, havespecialkey, havebook, havepepperspray, haveremote, havebirdSeed, havefreezeTrap, havewheelCrank, haverustyPadlockKey, havewoodenStick, havespiderKey, havechainCutter;
int LolId;
float speedfloat = 1;


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

#endif
