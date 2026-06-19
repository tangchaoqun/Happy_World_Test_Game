#include "MonsterManager.h"
#include "../Player/Player.h"
#include "../UI/HpMpBar.h"

USING_NS_CC;

MonsterManager* MonsterManager::s_instance = nullptr;

MonsterManager* MonsterManager::getInstance() {
    if (!s_instance) {
        s_instance = new MonsterManager();
    }
    return s_instance;
}

MonsterManager::MonsterManager() {
    registerMonsters();
}

MonsterManager::~MonsterManager() {
    for (auto& inst : m_instances) {
        delete inst.monsterData;
        delete inst.ai;
    }
    m_instances.clear();
}

void MonsterManager::registerMonsters() {
    m_monsterTemplates.emplace(1001, Monster(1001, "史莱姆", 1, 30, 8, 2, 15));
    m_monsterTemplates.emplace(1002, Monster(1002, "骷髅兵", 2, 45, 12, 4, 25));
    m_monsterTemplates.emplace(1003, Monster(1003, "蝙蝠", 1, 20, 10, 1, 12));
    m_monsterTemplates.emplace(1004, Monster(1004, "野狼", 3, 60, 15, 5, 35));
    m_monsterTemplates.emplace(1005, Monster(1005, "哥布林", 2, 35, 11, 3, 20));
    m_monsterTemplates.emplace(1006, Monster(1006, "兽人", 5, 120, 22, 10, 60));
}

Monster* MonsterManager::getMonsterTemplate(int monsterId) {
    auto it = m_monsterTemplates.find(monsterId);
    return it != m_monsterTemplates.end() ? &it->second : nullptr;
}

Sprite* MonsterManager::spawnMonsterInWorld(int monsterId, const Vec2& position, Node* parent) {
    auto it = m_monsterTemplates.find(monsterId);
    if (it == m_monsterTemplates.end()) return nullptr;

    // 创建怪物数据拷贝
    Monster* monsterData = new Monster(it->second);

    // 创建 Sprite
    auto tex = Director::getInstance()->getTextureCache()->addImage("monster.png");
    Sprite* sprite;
    if (tex) {
        sprite = Sprite::createWithTexture(tex);
    } else {
        sprite = Sprite::create();
        sprite->setTextureRect(Rect(0, 0, 28, 28));
        sprite->setColor(Color3B(255, 80, 80));
    }
    sprite->setPosition(position);
    sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    sprite->setUserData(monsterData);
    parent->addChild(sprite, 1);

    // 血条
    auto hpBar = HpMpBar::create(40, 4, Color3B::RED);
    hpBar->setPosition(Vec2(0, 22));
    hpBar->setTag(100);
    sprite->addChild(hpBar, 3);

    // 名字
    auto nameLabel = Label::createWithSystemFont(monsterData->getName(), "Arial", 10);
    nameLabel->setPosition(Vec2(0, 30));
    nameLabel->setTextColor(Color4B(255, 150, 150, 255));
    nameLabel->setTag(101);
    sprite->addChild(nameLabel, 3);

    // 创建 AI
    MonsterAI* ai = new MonsterAI(sprite);
    ai->setSpawnPosition(position);

    MonsterInstance inst;
    inst.templateId = monsterId;
    inst.monsterData = monsterData;
    inst.ai = ai;
    inst.sprite = sprite;
    inst.dead = false;

    m_instances.push_back(inst);

    return sprite;
}

void MonsterManager::updateAllAI(float dt, const Vec2& playerPos, bool playerAlive) {
    for (auto& inst : m_instances) {
        if (inst.dead) continue;
        if (inst.ai) {
            inst.ai->update(dt, playerPos, playerAlive);
        }
        // 更新血条
        if (inst.sprite && inst.monsterData) {
            auto hpBar = dynamic_cast<HpMpBar*>(inst.sprite->getChildByTag(100));
            if (hpBar && inst.ai) {
                float ratio = inst.ai->getMaxHp() > 0 ? inst.ai->getCurrentHp() / inst.ai->getMaxHp() : 0;
                hpBar->setHpRatio(ratio);
            }
        }
    }
}

void MonsterManager::damageMonster(Monster* monster, float damage, const Vec2& attackerPos) {
    for (auto& inst : m_instances) {
        if (inst.monsterData == monster && !inst.dead && inst.ai) {
            inst.ai->takeDamage(damage, attackerPos);
            // 受击闪烁
            if (inst.sprite && inst.ai && !inst.ai->isDead()) {
                inst.sprite->runAction(Sequence::create(
                    TintTo::create(0.05f, Color3B(255, 80, 80)),
                    TintTo::create(0.1f, Color3B::WHITE),
                    nullptr
                ));
            }
            break;
        }
    }
}

std::vector<Monster*> MonsterManager::getMonstersInRange(const Vec2& pos, float range) {
    std::vector<Monster*> result;
    for (auto& inst : m_instances) {
        if (inst.dead || !inst.sprite || !inst.monsterData) continue;
        if (inst.ai && inst.ai->isDead()) continue;
        float dist = pos.distance(inst.sprite->getPosition());
        if (dist <= range) {
            result.push_back(inst.monsterData);
        }
    }
    return result;
}

void MonsterManager::removeDeadMonsters() {
    for (auto it = m_instances.begin(); it != m_instances.end(); ) {
        bool shouldRemove = false;
        if (it->ai && it->ai->isReadyToRemove()) {
            shouldRemove = true;
        }
        if (shouldRemove) {
            // 给玩家加经验
            if (it->monsterData) {
                Player::getInstance()->addExp(it->monsterData->getExpReward());
            }
            // 先停止sprite上的所有动作，防止回调访问已删除的数据
            if (it->sprite) {
                it->sprite->stopAllActions();
                it->sprite->removeFromParent();
            }
            delete it->monsterData;
            delete it->ai;
            it = m_instances.erase(it);
        } else {
            ++it;
        }
    }
}

void MonsterManager::resetAll() {
    for (auto& inst : m_instances) {
        delete inst.monsterData;
        delete inst.ai;
        if (inst.sprite) inst.sprite->removeFromParent();
    }
    m_instances.clear();
    for (auto& pair : m_monsterTemplates) {
        pair.second.reset();
    }
}
