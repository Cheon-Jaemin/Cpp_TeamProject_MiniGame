#include "Shop.h"

void Shop::drawShop() {
    system("cls");
    cout << "=========================================\n";
    cout << "                 🏪 상점 🏪\n";
    cout << "=========================================\n";
    cout << R"(
        (\_ _/)   어서오게, 여행자!
        ( •w•)    좋은 물건들이 있지~
        / >🍗
    )" << endl;

    cout << "-----------------------------------------\n";
    cout << "1. 🗡 공격력 증가 아이템 (1 코인)\n";
    cout << "2. ❤ 체력 증가 아이템 (1 코인)\n";
    cout << "3. 🐾 귀여운 펫 (2 코인, 3초마다 10의 데미지)\n";
    cout << "-----------------------------------------\n";
    cout << "당신의 코인: " << coin << "\n";
    cout << "현재 공격력: " << playerAttack
        << " / 체력: " << playerHp
        << " / 펫: " << petCount << " 마리\n";
    cout << "-----------------------------------------\n";
    cout << "구매할 아이템 번호를 입력하세요 (0: 종료): ";
}

void Shop::enterShop() {
    while (true) {
        drawShop();
        int choice;
        cin >> choice;

        if (choice == 0) {
            cout << "상점을 떠납니다...\n";
            Sleep(3000);
            break;
        }
        else if (choice == 1) { buyAttack(); Sleep(3000); }
        else if (choice == 2) { buyHp(); Sleep(3000); }
        else if (choice == 3) { buyPet(); Sleep(3000); }
        else { cout << "잘못된 입력입니다.\n"; Sleep(3000); }

        cout << "계속하려면 아무 키나 누르세요...\n";
        _getch();
    }
}

void Shop::buyAttack() {
    if (coin >= 1) {
        coin -= 1;
        playerAttack++;
        cout << "🗡 공격력이 1 증가했습니다! 현재 공격력: " << playerAttack << "\n";
    }
    else cout << "코인이 부족합니다!\n";
}

void Shop::buyHp() {
    if (coin >= 1) {
        coin -= 1;
        playerHp += 10;
        cout << "❤ 체력이 10 증가했습니다! 현재 체력: " << playerHp << "\n";
    }
    else cout << "코인이 부족합니다!\n";
}

void Shop::buyPet() {
    if (coin >= 2) {
        coin -= 2;
        petCount++;
        cout << "🐾 펫을 샀습니다! 현재 펫 수: " << petCount << " 마리\n";

        if (petCount == 1)
            cout << "🐶 귀여운 펫이 당신을 따라다니기 시작합니다!\n";
        else if (petCount == 2)
            cout << "🐱 두 번째 펫도 합류했어요! 시끌벅적~\n";
        else if (petCount == 3)
            cout << "🐰 세 번째 펫이 뒤뚱뒤뚱 따라옵니다! 귀여움 +100\n";
        else
            cout << "🐾 새로운 펫이 무리를 이루어 당신을 따라다닙니다! (총 "
            << petCount << " 마리)\n";
    }
    else cout << "코인이 부족합니다!\n";
}