#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <android/log.h>

#define TAG "YrenerSoft"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

bool godMode=false, rapidFire=false, instantKill=false, aimHelper=false;

uintptr_t getLibBase(const char* name) {
    FILE* f = fopen("/proc/self/maps", "r");
    if (!f) return 0;
    char line[512];
    uintptr_t base = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, name) && strstr(line, "r-xp")) {
            base = (uintptr_t)strtoull(line, nullptr, 16);
            break;
        }
    }
    fclose(f);
    return base;
}

void* modThread(void*) {
    LOGI("╔══════════════════════════╗");
    LOGI("║  YRENER SOFT v1.0        ║");
    LOGI("║  tg: @Yrener_Soft        ║");
    LOGI("╠══════════════════════════╣");
    LOGI("║ [%s] God Mode            ║", godMode?"✓":" ");
    LOGI("║ [%s] Rapid Fire          ║", rapidFire?"✓":" ");
    LOGI("║ [%s] Instant Kill        ║", instantKill?"✓":" ");
    LOGI("║ [%s] Aim Helper          ║", aimHelper?"✓":" ");
    LOGI("╚══════════════════════════╝");

    uintptr_t base = 0;
    while (!base) { base = getLibBase("libil2cpp.so"); sleep(1); }
    LOGI("[+] libil2cpp.so: 0x%lX", base);

    while(true) { sleep(5); LOGI("[Yrener] активен"); }
    return nullptr;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("[*] Yrener Soft загружается...");
    pthread_t t;
    pthread_create(&t, nullptr, modThread, nullptr);
    pthread_detach(t);
    return JNI_VERSION_1_6;
}
