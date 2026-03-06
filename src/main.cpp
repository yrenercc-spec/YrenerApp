#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <android/log.h>

#define TAG "YrenerSoft"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

bool godMode     = false;
bool rapidFire   = false;
bool instantKill = false;
bool aimHelper   = false;
bool autoShoot   = false;

static JavaVM* g_jvm = nullptr;
static jobject g_activity = nullptr;

// Показываем меню через Java
void showMenu() {
    JNIEnv* env = nullptr;
    g_jvm->AttachCurrentThread(&env, nullptr);
    if (!env || !g_activity) return;

    jclass cls = env->GetObjectClass(g_activity);
    jmethodID mid = env->GetMethodID(cls, "runOnUiThread", "(Ljava/lang/Runnable;)V");
    if (!mid) { LOGI("[-] runOnUiThread не найден"); return; }

    // Вызываем наш Java класс меню
    jclass menuCls = env->FindClass("com/yrener/YrenerMenu");
    if (!menuCls) { LOGI("[-] YrenerMenu не найден"); return; }

    jmethodID showMid = env->GetStaticMethodID(menuCls, "show",
        "(Landroid/app/Activity;)V");
    if (!showMid) { LOGI("[-] show() не найден"); return; }

    env->CallStaticVoidMethod(menuCls, showMid, g_activity);
    LOGI("[+] Меню показано!");
}

void* modThread(void*) {
    LOGI("╔══════════════════════════╗");
    LOGI("║  YRENER SOFT v1.0        ║");
    LOGI("║  tg: @Yrener_Soft        ║");
    LOGI("╚══════════════════════════╝");

    sleep(2);
    showMenu();

    while(true) {
        sleep(5);
        LOGI("[Yrener] активен | god=%d rapid=%d kill=%d aim=%d",
            godMode, rapidFire, instantKill, aimHelper);
    }
    return nullptr;
}

// Activity получаем через UnityPlayer
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_jvm = vm;
    LOGI("[*] Yrener Soft загружается...");

    JNIEnv* env = nullptr;
    vm->GetEnv((void**)&env, JNI_VERSION_1_6);

    // Получаем Activity через UnityPlayer (Bowmasters = Unity)
    jclass up = env->FindClass("com/unity3d/player/UnityPlayer");
    if (up) {
        jfieldID fid = env->GetStaticFieldID(up,
            "currentActivity", "Landroid/app/Activity;");
        if (fid) {
            g_activity = env->NewGlobalRef(
                env->GetStaticObjectField(up, fid));
            LOGI("[+] Activity получена!");
        }
    }

    pthread_t t;
    pthread_create(&t, nullptr, modThread, nullptr);
    pthread_detach(t);
    return JNI_VERSION_1_6;
}

// JNI функции которые вызывает Java меню
extern "C" {

JNIEXPORT void JNICALL
Java_com_yrener_YrenerMenu_setGodMode(JNIEnv*, jclass, jboolean val) {
    godMode = val;
    LOGI("[GodMode] %s", godMode ? "ВКЛ" : "ВЫКЛ");
}

JNIEXPORT void JNICALL
Java_com_yrener_YrenerMenu_setRapidFire(JNIEnv*, jclass, jboolean val) {
    rapidFire = val;
    LOGI("[RapidFire] %s", rapidFire ? "ВКЛ" : "ВЫКЛ");
}

JNIEXPORT void JNICALL
Java_com_yrener_YrenerMenu_setInstantKill(JNIEnv*, jclass, jboolean val) {
    instantKill = val;
    LOGI("[InstantKill] %s", instantKill ? "ВКЛ" : "ВЫКЛ");
}

JNIEXPORT void JNICALL
Java_com_yrener_YrenerMenu_setAimHelper(JNIEnv*, jclass, jboolean val) {
    aimHelper = val;
    LOGI("[AimHelper] %s", aimHelper ? "ВКЛ" : "ВЫКЛ");
}

JNIEXPORT void JNICALL
Java_com_yrener_YrenerMenu_setAutoShoot(JNIEnv*, jclass, jboolean val) {
    autoShoot = val;
    LOGI("[AutoShoot] %s", autoShoot ? "ВКЛ" : "ВЫКЛ");
}

} // extern "C"
