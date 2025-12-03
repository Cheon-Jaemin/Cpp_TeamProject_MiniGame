#include "BossBattle.h"



BossBattle::BossBattle() {
    nowHp = playerHp;
    lastBossAttack = clock();
    lastPetAttack = clock();
}

void BossBattle::clearScreens() {
    system("cls");
}

void BossBattle::drawHPBar(const string& name, int hp, int maxHP, int barWidth) {
    cout << name << " [";
    int filled = (double)hp / maxHP * barWidth;
    for (int i = 0; i < barWidth; i++)
        cout << (i < filled ? "■" : " ");
    cout << "] " << hp << "/" << maxHP << "\n";
}

void BossBattle::drawBattle() {
    clearScreens();
    cout << "=================== ⚔ 보스 전투 ⚔ ===================\n\n";

    cout << R"(
       /^^^^^^^^^^^\
      /             \
     |   (o)   (o)   |
     |       ^       |
     |     '-'       |
      \   \___/     /
       \___________/
)";

    drawHPBar("보스 HP", bossHP, 1000);
    cout << "\n";

    drawHPBar("플레이어 HP", nowHp, playerHp);
    cout << "플레이어 공격력: " << playerAttack << "\n";
    cout << "펫 수: " << petCount << " (3초마다 " << (petCount * 30) << " 데미지)\n";
    cout << "-----------------------------------------------------\n";
    cout << "스페이스바: 공격   |   보스는 3초마다 공격합니다!\n";
    cout << "=====================================================\n";
}

void BossBattle::startBattle() {
    bool spacePressedPrev = false;
    clock_t lastDraw = 0;

    while (nowHp > 0 && bossHP > 0) {
        bool spacePressedNow = (GetAsyncKeyState(VK_SPACE) & 0x8000);

        if (spacePressedNow && !spacePressedPrev) {
            bossHP -= playerAttack;
            if (bossHP < 0) bossHP = 0;
            cout << ">> 당신이 공격하여 보스에게 " << playerAttack << " 데미지를 입혔습니다!\n";
        }
        spacePressedPrev = spacePressedNow;

        if ((clock() - lastBossAttack) / CLOCKS_PER_SEC >= 3) {
            nowHp -= bossAttack;
            if (nowHp < 0) nowHp = 0;
            cout << "🔥 보스가 강력한 공격을 가해 " << bossAttack << " 피해를 입었습니다!\n";
            lastBossAttack = clock();
        }

        if (petCount > 0 && (clock() - lastPetAttack) / CLOCKS_PER_SEC >= 3) {
            int petDamage = petCount * 30;
            bossHP -= petDamage;
            if (bossHP < 0) bossHP = 0;
            cout << "🐾 펫들이 달려들어 보스에게 " << petDamage << " 피해를 입혔습니다!\n";
            lastPetAttack = clock();
        }

        if ((clock() - lastDraw) / (double)CLOCKS_PER_SEC >= 0.2) {
            drawBattle();
            lastDraw = clock();
        }

        Sleep(10);
    }

    drawBattle();
    if (bossHP <= 0) cout << "\n🎉 승리했습니다! 보스를 쓰러뜨렸습니다!\n";
    else cout << "\n💀 패배했습니다... 플레이어가 쓰러졌습니다...\n";

    Sleep(3000);
    system("pause");
}