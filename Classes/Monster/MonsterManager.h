#ifndef __MONSTER_MANAGER_H__
#define __MONSTER_MANAGER_H__

#include "cocos2d.h"
#include "Monster.h"
#include "MonsterAI.h"
#include <unordered_map>
#include <vector>

// 管理怪物实例、AI 和 Sprite 的完整生命周期
class MonsterManager {
public:
    struct MonsterInstance {
        int templateId = 0;        // 怪物模板 ID
        Monster* monsterData = nullptr;  // 纯数据
        MonsterAI* ai = nullptr;        // AI 控制器
        cocos2d::Sprite* sprite = nullptr;  // 场景精灵
        float respawnTimer = 0.0f;
        bool dead = false;
    };

    static MonsterManager* getInstance();

    // 在世界中生成一只怪物，返回 Sprite（用于添加到场景）
    cocos2d::Sprite* spawnMonsterInWorld(int monsterId, const cocos2d::Vec2& position, cocos2d::Node* parent);

    // 更新所有怪物 AI
    void updateAllAI(float dt, const cocos2d::Vec2& playerPos, bool playerAlive);

    // 对某只怪物造成伤害（通过 Monster 数据指针查找）
    void damageMonster(Monster* monster, float damage, const cocos2d::Vec2& attackerPos);

    // 获取指定范围内活着的怪物列表
    std::vector<Monster*> getMonstersInRange(const cocos2d::Vec2& pos, float range);

    // 移除所有死亡怪物
    void removeDeadMonsters();

    // 获取所有活着的怪物 Sprite
    const std::vector<MonsterInstance>& getAllInstances() const { return m_instances; }

    // 获取怪物模板
    Monster* getMonsterTemplate(int monsterId);

    void resetAll();

private:
    MonsterManager();
    ~MonsterManager();

    void registerMonsters();

    static MonsterManager* s_instance;
    std::unordered_map<int, Monster> m_monsterTemplates;
    std::vector<MonsterInstance> m_instances;
    int m_nextId = 1;
};

#endif // __MONSTER_MANAGER_H__
