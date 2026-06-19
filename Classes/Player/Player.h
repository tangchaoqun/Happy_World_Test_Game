#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include <string>
#include <vector>

// 装备类型
enum class EquipmentType {
    Weapon,     // 武器
    Armor,      // 铠甲
    Helmet,     // 头盔
    Ring,       // 戒指
    Necklace,   // 项链
    Shoes,      // 鞋子
    MAX_COUNT
};

// 装备稀有度
enum class Rarity {
    Common,     // 普通
    Uncommon,   // 优秀
    Rare,       // 稀有
    Epic,       // 史诗
    Legendary   // 传说
};

// 装备属性结构
struct EquipmentStats {
    int attack = 0;
    int defense = 0;
    int hp = 0;
    int mp = 0;
    int strength = 0;
    int agility = 0;
    int intelligence = 0;
    int critRate = 0;    // 暴击率 (万分比)
    int critDamage = 0;  // 暴击伤害 (万分比)
};

// 装备
struct Equipment {
    int id = 0;
    std::string name;
    std::string description;
    EquipmentType type = EquipmentType::Weapon;
    Rarity rarity = Rarity::Common;
    EquipmentStats stats;
    int level = 1;
    int price = 0;        // 出售价格
    std::string icon;     // 图标资源名
};

// 物品
struct Item {
    int id = 0;
    std::string name;
    std::string description;
    int stack = 1;        // 堆叠数量
    int maxStack = 99;    // 最大堆叠
    bool usable = false;  // 是否可使用
    std::string useEffect; // 使用效果 (JSON)
    int price = 0;
    std::string icon;
};

// 技能
struct Skill {
    int id = 0;
    std::string name;
    std::string description;
    int level = 1;
    int maxLevel = 10;
    float cd = 0;           // 冷却时间(秒)
    float currentCd = 0;
    int mpCost = 0;         // 消耗MP
    float damageMultiplier = 1.0f;
    std::string icon;
};

class Player {
public:
    static Player* getInstance();
    
    // 基础属性
    void setLevel(int lv);
    int getLevel() const { return m_level; }
    
    void setPlayerName(const std::string& name) { m_name = name; }
    const std::string& getPlayerName() const { return m_name; }
    
    void addExp(int exp);
    int getExp() const { return m_exp; }
    int getMaxExp() const;
    
    // 基础战斗属性
    int getMaxHp() const;
    int getMaxMp() const;
    int getAttack() const;
    int getDefense() const;
    int getStrength() const;
    int getAgility() const;
    int getIntelligence() const;
    int getCritRate() const;
    int getCritDamage() const;
    
    // 当前状态
    void setHp(int hp) { m_hp = std::max(0, std::min(hp, getMaxHp())); }
    int getHp() const { return m_hp; }
    void setMp(int mp) { m_mp = std::max(0, std::min(mp, getMaxMp())); }
    int getMp() const { return m_mp; }
    
    bool isAlive() const { return m_hp > 0; }
    
    // 受伤/治疗
    void takeDamage(int damage);
    void heal(int amount);
    void restoreMp(int amount);
    
    // 升级
    void levelUp();
    
    // 装备系统
    bool equip(int equipmentId);
    bool unequip(EquipmentType type);
    const Equipment* getEquipped(EquipmentType type) const;
    std::vector<Equipment> getEquippedList() const;
    
    // 背包系统
    void addItem(const Item& item, int count = 1);
    bool removeItem(int itemId, int count = 1);
    int getItemCount(int itemId) const;
    const std::vector<std::pair<Item, int>>& getInventory() const { return m_inventory; }
    void useItem(int itemId);
    
    // 技能系统
    void addSkill(const Skill& skill);
    void upgradeSkill(int skillId);
    const std::vector<Skill>& getSkills() const { return m_skills; }
    bool canUseSkill(int skillId) const;
    void useSkill(int skillId);
    void updateSkills(float dt);
    
    // 序列化
    std::string toJson() const;
    void fromJson(const std::string& json);
    
    // 保存/加载
    void save();
    void load();

private:
    Player();
    ~Player() = default;
    
    EquipmentStats getBaseStats() const;
    EquipmentStats getEquipStats() const;
    EquipmentStats getTotalStats() const;
    
    static Player* s_instance;
    
    // 基础属性
    std::string m_name;
    int m_level = 1;
    int m_exp = 0;
    
    // 当前状态
    int m_hp = 100;
    int m_mp = 50;
    
    // 装备栏 [0]=Weapon, [1]=Armor, ...
    Equipment m_equipped[static_cast<int>(EquipmentType::MAX_COUNT)];
    bool m_hasEquipped[static_cast<int>(EquipmentType::MAX_COUNT)] = {false};
    
    // 背包
    std::vector<std::pair<Item, int>> m_inventory;
    static constexpr int MAX_INVENTORY = 40;
    
    // 技能
    std::vector<Skill> m_skills;
    
    // 升级所需经验公式
    static constexpr int BASE_EXP = 100;
    static constexpr float EXP_GROWTH = 1.5f;
};

#endif // __PLAYER_H__
