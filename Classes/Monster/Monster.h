#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include <string>

class Monster {
public:
    Monster(int id, const std::string& name, int level, int hp, int attack, int defense, int expReward);
    
    int getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    int getLevel() const { return m_level; }
    int getHp() const { return m_hp; }
    int getMaxHp() const { return m_maxHp; }
    int getAttack() const { return m_attack; }
    int getDefense() const { return m_defense; }
    int getExpReward() const { return m_expReward; }
    bool isAlive() const { return m_hp > 0; }
    float getMoveSpeed() const { return m_moveSpeed; }
    void setMoveSpeed(float speed) { m_moveSpeed = speed; }
    
    void setPosition(const cocos2d::Vec2& pos) { m_position = pos; }
    cocos2d::Vec2 getPosition() const { return m_position; }
    
    void takeDamage(int damage);
    void reset();

private:
    int m_id;
    std::string m_name;
    int m_level;
    int m_maxHp;
    int m_hp;
    int m_attack;
    int m_defense;
    int m_expReward;
    float m_moveSpeed = 80.0f;
    cocos2d::Vec2 m_position;
};

#endif // __MONSTER_H__
