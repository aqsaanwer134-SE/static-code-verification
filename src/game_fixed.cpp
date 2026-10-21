// game_fixed.cpp
// Fixed version of game.cpp after static verification.
// Every change is explained in reports/table2_code_review_report.md

#include <iostream>
#include <string>

const int MAX_MONSTERS = 5;

class Monster {
private:
    int id_;
    std::string name_;
    int health_;
    int attack_;

public:
    Monster(int id, const std::string& name, int health, int attack)
        : id_(id), name_(name), health_(health), attack_(attack) {}

    int getId() const { return id_; }
    const std::string& getName() const { return name_; }
    int getHealth() const { return health_; }
    int getAttack() const { return attack_; }

    // health never goes below 0
    void takeDamage(int damage) {
        health_ -= damage;
        if (health_ < 0)
            health_ = 0;
    }
};

class Game {
private:
    Monster* monsters[MAX_MONSTERS] = {};
    int monsterCount = 0;

    void clear() {
        for (int i = 0; i < monsterCount; i++) {
            delete monsters[i];
            monsters[i] = nullptr;
        }
        monsterCount = 0;
    }

public:
    Game() = default;

    // Rule of three: deep copy so two Game objects never share monsters
    Game(const Game& other) {
        for (int i = 0; i < other.monsterCount; i++)
            monsters[i] = new Monster(*other.monsters[i]);
        monsterCount = other.monsterCount;
    }

    Game& operator=(const Game& other) {
        if (this != &other) {
            clear();
            for (int i = 0; i < other.monsterCount; i++)
                monsters[i] = new Monster(*other.monsters[i]);
            monsterCount = other.monsterCount;
        }
        return *this;
    }

    ~Game() { clear(); }

    // returns false if the game is already full
    bool spawnMonster(int id, const std::string& name, int health, int attack) {
        if (monsterCount >= MAX_MONSTERS)
            return false;
        monsters[monsterCount] = new Monster(id, name, health, attack);
        monsterCount++;
        return true;
    }

    Monster* findMonster(int id) {
        for (int i = 0; i < monsterCount; i++) {
            if (monsters[i]->getId() == id)
                return monsters[i];
        }
        return nullptr;
    }

    // returns remaining health, or -1 if the monster does not exist
    int attackMonster(int id, int damage) {
        Monster* m = findMonster(id);
        if (m == nullptr)
            return -1;
        if (damage > 0)
            m->takeDamage(damage);
        return m->getHealth();
    }

    int totalMonsterHealth() const {
        int total = 0;
        for (int i = 0; i < monsterCount; i++)
            total += monsters[i]->getHealth();
        return total;
    }

    std::string getMonsterLabel(int id) {
        const Monster* m = findMonster(id);
        if (m == nullptr)
            return "Monster-unknown";
        return "Monster-" + std::to_string(m->getId()) + ":" + m->getName();
    }

    double averageAttack() const {
        if (monsterCount == 0)
            return 0.0;
        double sum = 0.0;
        for (int i = 0; i < monsterCount; i++)
            sum += monsters[i]->getAttack();
        return sum / monsterCount;
    }

    // returns false if the monster does not exist
    bool defeatMonster(int id) {
        for (int i = 0; i < monsterCount; i++) {
            if (monsters[i]->getId() == id) {
                std::cout << "Defeated " << monsters[i]->getName() << "\n";
                delete monsters[i];
                // shift the rest so there is no dangling pointer in the array
                for (int j = i; j < monsterCount - 1; j++)
                    monsters[j] = monsters[j + 1];
                monsters[monsterCount - 1] = nullptr;
                monsterCount--;
                return true;
            }
        }
        return false;
    }
};

double calculateDamage(int attack, int armorPercent) {
    return attack * armorPercent / 100.0;
}

int playerRank(int score) {
    if (score > 1000)
        return 1;
    else if (score > 100)
        return 2;
    return 3;
}

void readBoss(Game& game) {
    std::string name;
    int health = 0;
    std::cout << "Enter boss name: ";
    std::cin >> name;
    std::cout << "Enter boss health: ";
    if (!(std::cin >> health)) {
        std::cout << "Invalid health, boss not added.\n";
        return;
    }
    if (!game.spawnMonster(100, name, health, 50))
        std::cout << "Game is full, boss not added.\n";
}

int main() {
    Game game;
    game.spawnMonster(1, "Goblin", 30, 5);
    game.spawnMonster(2, "Orc", 60, 12);
    game.spawnMonster(3, "Skeleton", 40, 8);
    game.spawnMonster(4, "Troll", 90, 20);
    game.spawnMonster(5, "Dragon", 200, 45);
    if (!game.spawnMonster(6, "Bat", 10, 3))
        std::cout << "Could not spawn Bat: game is full\n";

    Monster bonus(0, "Slime", 1, 0);   // normal object, no leak
    std::cout << "Bonus monster: " << bonus.getName() << "\n";

    std::cout << "Attack: " << game.attackMonster(99, 20) << "\n";
    std::cout << "Label: " << game.getMonsterLabel(2) << "\n";
    std::cout << "Total health: " << game.totalMonsterHealth() << "\n";
    std::cout << "Average attack: " << game.averageAttack() << "\n";
    std::cout << "Damage: " << calculateDamage(80, 25) << "\n";
    std::cout << "Rank: " << playerRank(50) << "\n";

    game.defeatMonster(3);
    readBoss(game);

    const Game copy = game;   // deep copy now
    std::cout << "Copy total health: " << copy.totalMonsterHealth() << "\n";
    return 0;
}
