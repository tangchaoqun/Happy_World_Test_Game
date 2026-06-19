#include "Monster.h"

Monster::Monster(int id, const std::string& name, int level, int hp, int attack, int defense, int expReward)
    : m_id(id), m_name(name), m_level(level), m_maxHp(hp), m_hp(hp),
      m_attack(attack), m_defense(defense), m_expReward(expReward) {}

void Monster::takeDamage(int damage) {
    int actualDmg = std::max(1, damage - m_defense / 2);
    m_hp = std::max(0, m_hp - actualDmg);
}

void Monster::reset() {
    m_hp = m_maxHp;
}
