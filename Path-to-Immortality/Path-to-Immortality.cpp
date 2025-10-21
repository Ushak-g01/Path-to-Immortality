#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Stage {
    string name;
    int requiredExp;
    int currentExp;
};

struct Level {
    string name;
    vector<Stage> stages;
};

struct Item {
    string name;
    int quantity;
};

struct Resource {
    string name;
    double chance;
    int minQuantity;
    int maxQuantity;
};

vector<Level> initLevels() {
    vector<Level> levels(5);

    vector<vector<string>> stageNames = {
        {"Начальный", "Средний", "Поздний"},
        {"Начальный", "Средний", "Поздний"},
        {"Начальный", "Средний", "Поздний"},
        {"Начальный", "Средний", "Поздний"},
        {"Начальный", "Средний", "Поздний"}
    };

    vector<vector<int>> stageExpReq = {
        {100, 250, 500},
        {1000, 2500, 5000},
        {5000, 12500, 25000},
        {60000, 150000, 300000},
        {500000, 1250000, 2500000}

    };

    vector<string> levelNames = {
        "Концентрация Ци",
        "Заложение Основ",
        "Золотое Ядро",
        "Зарождающаяся душа",
        "Обожествление"
    };

    for (int i = 0; i < 5; i++) {
        levels[i].name = levelNames[i];
        levels[i].stages.resize(3);
        for (int j = 0; j < 3; j++) {
            levels[i].stages[j].name = stageNames[i][j];
            levels[i].stages[j].requiredExp = stageExpReq[i][j];
            levels[i].stages[j].currentExp = 0;
        }
    }
    return levels;
}

void showProgress(const vector<Level>& levels, int levelIdx, int stageIdx) {
    cout << "======== Прогресс ========\n";
    cout << "Уровень: " << levels[levelIdx].name << "\n";
    cout << "Этап: " << levels[levelIdx].stages[stageIdx].name
        << " (" << levels[levelIdx].stages[stageIdx].currentExp
        << "/" << levels[levelIdx].stages[stageIdx].requiredExp << ")\n";
    cout << "=========================\n";
}

void addExp(vector<Level>& levels, int& levelIdx, int& stageIdx, int exp) {
    auto& stage = levels[levelIdx].stages[stageIdx];
    stage.currentExp += exp;
    cout << "Добавлено " << exp << " опыта к этапу " << stage.name << "\n";

    while (true) {
        if (stage.currentExp >= stage.requiredExp) {
            stage.currentExp -= stage.requiredExp;
            if (stageIdx < 2) {
                stageIdx++;
                stage = levels[levelIdx].stages[stageIdx];
                cout << "Вы перешли к этапу: " << stage.name << " уровня " << levels[levelIdx].name << "\n";
            }
            else {
                if (levelIdx < static_cast<int>(levels.size()) - 1) {
                    levelIdx++;
                    stageIdx = 0;
                    stage = levels[levelIdx].stages[stageIdx];
                    cout << "Поздравляю! Вы достигли нового уровня: " << levels[levelIdx].name << "\n";
                }
                else {
                    cout << "Вы достигли максимального уровня развития!\n";
                    break;
                }
            }
        }
        else {
            break;
        }
    }
}

struct Location {
    string name;
    vector<int> neighbors;
    vector<Resource> resources;
};

vector<Location> initMap() {
    vector<Location> map(5);

    map[0].name = "Лесная чаща";
    map[1].name = "Горная пещера";
    map[2].name = "Хребет дьявола";
    map[3].name = "Храм хаотичных небес";
    map[4].name = "Руины храма возвышения";

    map[0].neighbors = { 1, 2 };
    map[1].neighbors = { 0, 3 };
    map[2].neighbors = { 0, 4 };
    map[3].neighbors = { 1 };
    map[4].neighbors = { 3 };

    map[0].resources = {
        {"Духовные травы", 0.6, 1, 6},
        {"Древесный дух", 0.05, 1, 1},
        {"Живой мох", 0.9, 1, 8}
    };
    map[1].resources = {
        {"Нефрит Ян", 0.1, 1, 2},
        {"Драконьи кости", 0.3, 1, 3},
        {"Пламя-сердце", 0.25, 1, 4}
    };
    map[2].resources = {
        {"Тёмные кристаллы", 0.7, 2, 10},
        {"Дьяволийья Гонг", 0.1, 1, 2},
        {"Демонические кости", 0.4, 1, 5}
    };
    map[3].resources = {
        {"Облачно-ветреный жемчуг", 0.4, 1, 9},
        {"Молнии-иглы", 0.2, 6, 20},
        {"Летучие руны", 0.1, 3, 6}
    };
    map[4].resources = {
        {"Древние духовые камни", 0.85, 10, 30},
        {"Фрагменты небесных куполов", 0.1, 1, 1},
        {"Энергетические кристаллы времени", 0.4, 3, 5}
    };

    return map;
}

vector<Item> inventory;

void showCurrentLocation(const Location& loc) {
    cout << "Вы находитесь в: " << loc.name << endl;
}

int moveToLocation(const vector<Location>& map, int currentIdx) {
    const Location& currentLoc = map[currentIdx];

    if (currentLoc.neighbors.empty()) {
        cout << "Нет доступных соседних локаций для перемещения.\n";
        return currentIdx;
    }

    cout << "Доступные локации для перемещения:\n";
    for (int i = 0; i < currentLoc.neighbors.size(); i++) {
        int neighborIdx = currentLoc.neighbors[i];
        cout << i + 1 << ". " << map[neighborIdx].name << endl;
    }

    int choice;
    cout << "Введите номер выбранной локации: ";
    cin >> choice;

    if (choice < 1 || choice > currentLoc.neighbors.size()) {
        cout << "Некорректный выбор.\n";
        return currentIdx;
    }

    int newIdx = currentLoc.neighbors[choice - 1];
    return newIdx;
}

void showInventory() {
    cout << "====== Инвентарь ======\n";
    if (inventory.empty()) {
        cout << "Инвентарь пуст.\n";
    }
    else {
        for (const auto& item : inventory) {
            cout << item.name << " x" << item.quantity << "\n";
        }
    }
    cout << "======================\n";
}

void addItem(const string& itemName, int qty = 1) {
    auto it = find_if(inventory.begin(), inventory.end(), [&](const Item& item) {
        return item.name == itemName;
        });
    if (it != inventory.end()) {
        it->quantity += qty;
    }
    else {
        inventory.push_back({ itemName, qty });
    }
    cout << "Добавлен предмет: " << itemName << " x" << qty << "\n";
}

void useItem(const string& itemName, int qty = 1) {
    auto it = find_if(inventory.begin(), inventory.end(), [&](const Item& item) {
        return item.name == itemName;
        });
    if (it != inventory.end() && it->quantity >= qty) {
        it->quantity -= qty;
        cout << "Использован предмет: " << itemName << " x" << qty << "\n";
        if (it->quantity == 0) {
            inventory.erase(it);
        }
    }
    else {
        cout << "Недостаточно предметов или предмет не найден.\n";
    }
}
void collectResources(Location& loc) {
    if (loc.resources.empty()) {
        cout << "В этой локации нет ресурсов для сбора.\n";
        return;
    }
    for (auto& res : loc.resources) {
        double roll = (double)rand() / RAND_MAX;
        if (roll <= res.chance) {
            int quantity = res.minQuantity + rand() % (res.maxQuantity - res.minQuantity + 1);
            addItem(res.name, quantity);
            cout << "Вы успешно собрали " << quantity << " " << res.name << ".\n";
        }
        else {
            cout << "Не удалось собрать " << res.name << ".\n";
        }
    }
    loc.resources.clear();
}

int main() {
    setlocale(LC_ALL, "");
    vector<Level> levels = initLevels();

    int currentLevelIdx = 0;
    int currentStageIdx = 0;

    showProgress(levels, currentLevelIdx, currentStageIdx);

    vector<Location> map = initMap();
    int currentLocationIdx = 0;

    while (true) {
        showCurrentLocation(map[currentLocationIdx]);

        cout << "Что хотите сделать?\n";
        cout << "1. Переместиться\n";
        cout << "2. Добыть ресурсы\n";
        cout << "3. Посмотреть инвентарь\n";
        cout << "4. Выйти\n";

        int action;
        cin >> action;
        if (action == 1) {
            currentLocationIdx = moveToLocation(map, currentLocationIdx);
        }
        else if (action == 2) {
            collectResources(map[currentLocationIdx]);
        }
        else if (action == 3) {
            showInventory();
        }
        else if (action == 4) {
            break;
        }
        else {
            cout << "Некорректный выбор.\n";
        }
    }
}