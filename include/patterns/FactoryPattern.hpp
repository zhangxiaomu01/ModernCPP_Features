#pragma once

#include<iostream>
#include<vector>
#include<string>

namespace FactoryPatternNameSpace {
    //4. Factory Method Pattern
    //Factory Method
    //The factory method pattern defines an interface for creating an object, 
    //but let the subclasses to decide which classes to instantiate. Factory method 
    //let the classes defer instantiation to subclasses.
    //Factory Method Pattern:
    //Different factories create different enemy combinations

    //Factory Method Pattern:
    //Different factories create different enemy combinations

    //Product base class (abstract, in our case, product is enmemy)
    class Enemy {
    public:
        int m_ID;
        int m_HP;
        int m_Damage;
    public:
        Enemy(int id, int HP, int damage) {
            m_ID = id;
            m_HP = HP;
            m_Damage = damage;
        }
        virtual void EnemyAbility() = 0;
    };

    //Factory base class (in our case, factory is enemy spawner)
    class EnemySpawnFactory {
    public:
        void showEnemy(std::vector<Enemy*>& V) {
            for (int i = 0; i < V.size(); ++i) {
                std::cout << "Enemy " << V[i]->m_ID << " spawned!" << std::endl;
                std::cout << "HP: " << V[i]->m_HP << std::endl;
                std::cout << "Damage: " << V[i]->m_Damage << std::endl;
                V[i]->EnemyAbility();
            }
        }
        //Different factory will define its specific spawn rule
        virtual std::vector<Enemy*> SpawnEnemy() = 0;
    };


    //Product concrete implementation
    class Goblin : public Enemy {
    public:
        Goblin(int ID, int HP, int Damage) : Enemy(ID, HP, Damage){}
        void EnemyAbility() {
            std::cout << "Goblin can sneak!" << std::endl;
        }
    };

    class Orc : public Enemy {
    public:
        Orc(int ID, int HP, int Damage) : Enemy(ID, HP, Damage) {}
        void EnemyAbility() {
            std::cout << "Orcs can have blood rage!" << std::endl;
        }
    };

    class Giant : public Enemy {
    public:
        Giant(int ID, int HP, int Damage) : Enemy(ID, HP, Damage) {}
        void EnemyAbility() {
            std::cout << "Orcs can have devastating power!" << std::endl;
        }
    };

    //Concrete implementation of Factory, here we have different enemy spawner for different levels
    //Actually the concrete factory should be able to create concrete product
    class SpawnerLevel1 : public EnemySpawnFactory {
    private:
        //A set of enemies
        std::vector<Enemy*> res;
    public:
        //Spawn 2 goblins and 1 orc: actually define the spawn rule
        std::vector<Enemy*> SpawnEnemy() {
            Enemy* gob01 = new Goblin(1, 10, 3);
            Enemy* gob02 = new Goblin(2, 8, 5);
            Enemy* orc = new Orc(3, 20, 7);
            res.push_back(gob01);
            res.push_back(gob02);
            res.push_back(orc);
            showEnemy(res);
            return res;
        }
        virtual ~SpawnerLevel1() {
            for (auto it : res)
                delete it;
        }
    };

    class SpawnerLevel2 : public EnemySpawnFactory {
    private:
        //A set of enemies
        std::vector<Enemy*> res;
    public:
        //Spawn 1 goblins, 1 orc and 1 giant
        std::vector<Enemy*> SpawnEnemy() {
            Enemy* gob = new Goblin(4, 12, 1);
            Enemy* orc = new Orc(5, 20, 7);
            Enemy* giant = new Giant(6, 35, 15);
            res.push_back(gob);
            res.push_back(orc);
            res.push_back(giant);
            showEnemy(res);
            return res;
        }
        virtual ~SpawnerLevel2() {
            for (auto it : res)
                delete it;
        }
    };


    void TestDesignPattern() {
        SpawnerLevel1 spawnL1;
        spawnL1.SpawnEnemy();
        SpawnerLevel2 spawnL2;
        spawnL2.SpawnEnemy();

        system("pause");
    }


    //Abstract Factory Pattern
    //Abstract Factory pattern provides an interface for creating families of 
    //related or dependent objects without specifying their concrete classes. 
    //(we can create multiple objects)
    //This pattern is similar to Factory Method pattern except it creates families of 
    //related or dependent objects
    //Please refer to the URMs 
    //Omit for now
}