#include "Player.h"
#include <sstream>
#include <algorithm>

USING_NS_CC;

Player* Player::s_instance = nullptr;

Player* Player::getInstance() {
    if (!s_instance) {
        s_instance = new Player();
    }
    return s_instance;
}

Player::Player() {
    // Initialize empty equipment slots
    for (int i = 0; i < static_cast<int>(EquipmentType::MAX_COUNT); ++i) {
        m_hasEquipped[i] = false;
    }
    // Load saved data
    load();
}

// ---- 等级系统 ----

void Player::setLevel(int lv) {
    m_level = std::max(1, lv);
    // Recalculate HP/MP on level change
    if (m_hp > getMaxHp()) m_hp = getMaxHp();
    if (m_mp > getMaxMp()) m_mp = getMaxMp();
}

void Player::addExp(int exp) {
    m_exp += exp;
    while (m_exp >= getMaxExp()) {
        m_exp -= getMaxExp();
        levelUp();
    }
}

int Player::getMaxExp() const {
    return static_cast<int>(BASE_EXP * std::pow(EXP_GROWTH, m_level - 1));
}

// ---- 战斗属性 ----

EquipmentStats Player::getBaseStats() const {
    EquipmentStats s;
    // Base stats grow with level
    s.hp = 80 + m_level * 20;
    s.mp = 40 + m_level * 10;
    s.attack = 10 + m_level * 3;
    s.defense = 5 + m_level * 2;
    s.strength = 5 + m_level * 2;
    s.agility = 5 + m_level * 2;
    s.intelligence = 5 + m_level * 2;
    s.critRate = 500;     // 5% base crit rate (out of 10000)
    s.critDamage = 5000;  // 50% base crit damage
    return s;
}

EquipmentStats Player::getEquipStats() const {
    EquipmentStats s;
    for (int i = 0; i < static_cast<int>(EquipmentType::MAX_COUNT); ++i) {
        if (m_hasEquipped[i]) {
            s.attack += m_equipped[i].stats.attack;
            s.defense += m_equipped[i].stats.defense;
            s.hp += m_equipped[i].stats.hp;
            s.mp += m_equipped[i].stats.mp;
            s.strength += m_equipped[i].stats.strength;
            s.agility += m_equipped[i].stats.agility;
            s.intelligence += m_equipped[i].stats.intelligence;
            s.critRate += m_equipped[i].stats.critRate;
            s.critDamage += m_equipped[i].stats.critDamage;
        }
    }
    return s;
}

EquipmentStats Player::getTotalStats() const {
    EquipmentStats base = getBaseStats();
    EquipmentStats equip = getEquipStats();
    EquipmentStats total;
    total.hp = base.hp + equip.hp;
    total.mp = base.mp + equip.mp;
    total.attack = base.attack + equip.attack;
    total.defense = base.defense + equip.defense;
    total.strength = base.strength + equip.strength;
    total.agility = base.agility + equip.agility;
    total.intelligence = base.intelligence + equip.intelligence;
    total.critRate = base.critRate + equip.critRate;
    total.critDamage = base.critDamage + equip.critDamage;
    return total;
}

int Player::getMaxHp() const { return getTotalStats().hp; }
int Player::getMaxMp() const { return getTotalStats().mp; }
int Player::getAttack() const { return getTotalStats().attack; }
int Player::getDefense() const { return getTotalStats().defense; }
int Player::getStrength() const { return getTotalStats().strength; }
int Player::getAgility() const { return getTotalStats().agility; }
int Player::getIntelligence() const { return getTotalStats().intelligence; }
int Player::getCritRate() const { return getTotalStats().critRate; }
int Player::getCritDamage() const { return getTotalStats().critDamage; }

// ---- 状态管理 ----

void Player::takeDamage(int damage) {
    int actualDmg = std::max(1, damage - getDefense() / 2);
    m_hp = std::max(0, m_hp - actualDmg);
}

void Player::heal(int amount) {
    m_hp = std::min(getMaxHp(), m_hp + amount);
}

void Player::restoreMp(int amount) {
    m_mp = std::min(getMaxMp(), m_mp + amount);
}

void Player::levelUp() {
    m_level++;
    // Full heal on level up
    m_hp = getMaxHp();
    m_mp = getMaxMp();
    log("Level Up! Now level %d", m_level);
}

// ---- 装备系统 ----

bool Player::equip(int equipmentId) {
    // Find in inventory
    auto it = std::find_if(m_inventory.begin(), m_inventory.end(),
        [equipmentId](const auto& pair) { return pair.first.id == equipmentId; });
    if (it == m_inventory.end()) return false;
    
    // Build Equipment from Item data
    const Item& item = it->first;
    Equipment newEquip;
    newEquip.id = item.id;
    newEquip.name = item.name;
    newEquip.description = item.description;
    newEquip.type = static_cast<EquipmentType>(item.id % static_cast<int>(EquipmentType::MAX_COUNT));
    newEquip.rarity = Rarity::Common;
    newEquip.level = 1;
    newEquip.price = item.price;
    newEquip.icon = item.icon;
    
    int typeIdx = static_cast<int>(newEquip.type);
    
    // Unequip current if exists
    if (m_hasEquipped[typeIdx]) {
        const Equipment& oldEquip = m_equipped[typeIdx];
        Item oldItem;
        oldItem.id = oldEquip.id;
        oldItem.name = oldEquip.name;
        oldItem.description = oldEquip.description;
        oldItem.price = oldEquip.price;
        oldItem.icon = oldEquip.icon;
        addItem(oldItem);
    }
    
    // Equip new
    m_equipped[typeIdx] = newEquip;
    m_hasEquipped[typeIdx] = true;
    
    // Remove from inventory
    if (--it->second <= 0) {
        m_inventory.erase(it);
    }
    
    return true;
}

bool Player::unequip(EquipmentType type) {
    int typeIdx = static_cast<int>(type);
    if (!m_hasEquipped[typeIdx]) return false;
    
    // Convert Equipment to Item for inventory
    const Equipment& equip = m_equipped[typeIdx];
    Item item;
    item.id = equip.id;
    item.name = equip.name;
    item.description = equip.description;
    item.price = equip.price;
    item.icon = equip.icon;
    
    addItem(item);
    m_hasEquipped[typeIdx] = false;
    return true;
}

const Equipment* Player::getEquipped(EquipmentType type) const {
    int idx = static_cast<int>(type);
    return m_hasEquipped[idx] ? &m_equipped[idx] : nullptr;
}

std::vector<Equipment> Player::getEquippedList() const {
    std::vector<Equipment> list;
    for (int i = 0; i < static_cast<int>(EquipmentType::MAX_COUNT); ++i) {
        if (m_hasEquipped[i]) {
            list.push_back(m_equipped[i]);
        }
    }
    return list;
}

// ---- 背包系统 ----

void Player::addItem(const Item& item, int count) {
    // Try to stack
    for (auto& pair : m_inventory) {
        if (pair.first.id == item.id && pair.second < pair.first.maxStack) {
            int add = std::min(count, pair.first.maxStack - pair.second);
            pair.second += add;
            count -= add;
            if (count <= 0) return;
        }
    }
    // Add new stack
    if (static_cast<int>(m_inventory.size()) < MAX_INVENTORY) {
        m_inventory.push_back({item, count});
    }
}

bool Player::removeItem(int itemId, int count) {
    auto it = std::find_if(m_inventory.begin(), m_inventory.end(),
        [itemId](const auto& pair) { return pair.first.id == itemId; });
    if (it == m_inventory.end() || it->second < count) return false;
    
    it->second -= count;
    if (it->second <= 0) {
        m_inventory.erase(it);
    }
    return true;
}

int Player::getItemCount(int itemId) const {
    auto it = std::find_if(m_inventory.begin(), m_inventory.end(),
        [itemId](const auto& pair) { return pair.first.id == itemId; });
    return it != m_inventory.end() ? it->second : 0;
}

void Player::useItem(int itemId) {
    // Parse useEffect JSON and apply
    removeItem(itemId);
}

// ---- 技能系统 ----

void Player::addSkill(const Skill& skill) {
    auto it = std::find_if(m_skills.begin(), m_skills.end(),
        [&skill](const Skill& s) { return s.id == skill.id; });
    if (it == m_skills.end()) {
        m_skills.push_back(skill);
    }
}

void Player::upgradeSkill(int skillId) {
    auto it = std::find_if(m_skills.begin(), m_skills.end(),
        [skillId](const Skill& s) { return s.id == skillId; });
    if (it != m_skills.end() && it->level < it->maxLevel) {
        it->level++;
        it->damageMultiplier = 1.0f + (it->level - 1) * 0.1f;
    }
}

bool Player::canUseSkill(int skillId) const {
    auto it = std::find_if(m_skills.begin(), m_skills.end(),
        [skillId](const Skill& s) { return s.id == skillId; });
    if (it == m_skills.end()) return false;
    return it->currentCd <= 0 && m_mp >= it->mpCost;
}

void Player::useSkill(int skillId) {
    auto it = std::find_if(m_skills.begin(), m_skills.end(),
        [skillId](const Skill& s) { return s.id == skillId; });
    if (it == m_skills.end() || !canUseSkill(skillId)) return;
    
    m_mp -= it->mpCost;
    it->currentCd = it->cd;
}

void Player::updateSkills(float dt) {
    for (auto& skill : m_skills) {
        if (skill.currentCd > 0) {
            skill.currentCd = std::max(0.0f, skill.currentCd - dt);
        }
    }
}

// ---- 序列化 ----

std::string Player::toJson() const {
    // Simple JSON serialization
    std::stringstream ss;
    ss << "{"
       << "\"level\":" << m_level << ","
       << "\"exp\":" << m_exp << ","
       << "\"hp\":" << m_hp << ","
       << "\"mp\":" << m_mp
       << "}";
    return ss.str();
}

void Player::fromJson(const std::string& json) {
    // Parse simple JSON - in production use a proper JSON library
    // For now just a stub
}

void Player::save() {
    auto path = FileUtils::getInstance()->getWritablePath() + "player_data.json";
    FileUtils::getInstance()->writeStringToFile(toJson(), path);
}

void Player::load() {
    auto path = FileUtils::getInstance()->getWritablePath() + "player_data.json";
    if (FileUtils::getInstance()->isFileExist(path)) {
        auto data = FileUtils::getInstance()->getStringFromFile(path);
        fromJson(data);
    }
}
