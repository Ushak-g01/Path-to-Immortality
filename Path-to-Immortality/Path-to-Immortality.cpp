#include <iostream>
#include <vector>
#include <string>
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
        {0, 250, 500},
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
};

vector<Location> initMap() {
    vector<Location> map(5);

    map[0].name = "Лес";
    map[1].name = "Горная вершина";
    map[2].name = "Речная долина";
    map[3].name = "Пустыня";
    map[4].name = "Древний храм";

    map[0].neighbors = { 1, 2 };      // Лес связан с Гора и Долиной
    map[1].neighbors = { 0, 3 };      // Вершина связана с Лесом и Пустыней
    map[2].neighbors = { 0, 4 };      // Долина связана с Лесом и Храмом
    map[3].neighbors = { 1 };         // Пустыня связана только с Вершиной
    map[4].neighbors = { 2 };         // Храм связан только с Долиной

    return map;
}

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

int main() {
    setlocale(LC_ALL, "");
    vector<Level> levels = initLevels();

    int currentLevelIdx = 0;
    int currentStageIdx = 0;

    showProgress(levels, currentLevelIdx, currentStageIdx);

    addExp(levels, currentLevelIdx, currentStageIdx, 120);
    showProgress(levels, currentLevelIdx, currentStageIdx);

    addExp(levels, currentLevelIdx, currentStageIdx, 100);
    showProgress(levels, currentLevelIdx, currentStageIdx);

    addExp(levels, currentLevelIdx, currentStageIdx, 500);
    showProgress(levels, currentLevelIdx, currentStageIdx);

    addExp(levels, currentLevelIdx, currentStageIdx, 500);
    showProgress(levels, currentLevelIdx, currentStageIdx);

    vector<Location> map = initMap();
    int currentLocationIdx = 0;

    while (true) {
        showCurrentLocation(map[currentLocationIdx]);

        cout << "Что хотите сделать?\n";
        cout << "1. Переместиться\n";
        cout << "2. Выйти\n";
        int action;
        cin >> action;

        if (action == 1) {
            currentLocationIdx = moveToLocation(map, currentLocationIdx);
        }
        else if (action == 2) {
            break;
        }
        else {
            cout << "Некорректный выбор.\n";
        }
    }

    return 0;
}