// ================================================
//  main.cpp — Yrener Soft v1.0
//  Bowmasters ARM64
//  ЭТАП 1: Просто меню без хуков
//  Проверяем что .so загружается в игру!
// ================================================
#include <jni.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <android/log.h>

#define TAG "YrenerMenu"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// ================================================
//  Состояния читов
// ================================================
bool godMode     = false;
bool rapidFire   = false;
bool instantKill = false;
bool aimHelper   = false;
bool autoShoot   = false;

// ================================================
//  Найти базовый адрес .so
// ================================================
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

// ================================================
//  Печатаем меню в логи
//  Смотреть через: adb logcat | grep YrenerSoft
// ================================================
void printMenu() {
    LOGI("╔══════════════════════════════════╗");
    LOGI("║      YRENER SOFT v1.0            ║");
    LOGI("║      tg: @Yrener_Soft            ║");
    LOGI("╠══════════════════════════════════╣");
    LOGI("║ [%s] God Mode                    ║", godMode     ? "✓" : " ");
    LOGI("║ [%s] Rapid Fire (5 стрел)        ║", rapidFire   ? "✓" : " ");
    LOGI("║ [%s] Instant Kill                ║", instantKill ? "✓" : " ");
    LOGI("║ [%s] Aim Helper (хедшот)         ║", aimHelper   ? "✓" : " ");
    LOGI("║ [%s] Auto Shoot                  ║", autoShoot   ? "✓" : " ");
    LOGI("╚══════════════════════════════════╝");
}

// ================================================
//  Главный поток мода
// ================================================
void* modThread(void*) {
    LOGI("╔══════════════════════════════╗");
    LOGI("║   YRENER SOFT ЗАПУЩЕН!       ║");
    LOGI("║   Bowmasters ARM64           ║");
    LOGI("║   tg: @Yrener_Soft           ║");
    LOGI("╚══════════════════════════════╝");

    // Ждём загрузки игры
    LOGI("[*] Ищем libil2cpp.so...");
    uintptr_t base = 0;
    int tries = 0;
    while (!base && tries < 30) {
        base = getLibBase("libil2cpp.so");
        if (!base) { sleep(1); tries++; }
    }

    if (base) {
        LOGI("[+] libil2cpp.so найден: 0x%lX", base);
        LOGI("[!] Offsets пока не заданы — хуки не активны");
        LOGI("[!] Следующий этап: найти offsets через Ghidra");
    } else {
        LOGE("[-] libil2cpp.so не найден за 30 секунд");
    }

    // Показываем меню
    printMenu();

    // Цикл — каждые 5 секунд показываем статус
    while (true) {
        sleep(5);
        LOGI("[Yrener] Мод активен | base=0x%lX", base);
    }

    return nullptr;
}

// ================================================
//  Точка входа
// ================================================
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("[*] JNI_OnLoad → Yrener Soft запускается...");
    pthread_t t;
    pthread_create(&t, nullptr, modThread, nullptr);
    pthread_detach(t);
    return JNI_VERSION_1_6;
}
