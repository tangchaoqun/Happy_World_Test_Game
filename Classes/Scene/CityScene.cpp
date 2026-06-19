#include "CityScene.h"
#include "../Player/Player.h"
#include "../Monster/MonsterManager.h"
#include "../Network/NetworkManager.h"

USING_NS_CC;

Scene* CityScene::createScene() {
    return CityScene::create();
}

bool CityScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto bg = LayerColor::create(Color4B(30, 35, 50, 255));
    this->addChild(bg);

    setupMap();
    setupPlayer();
    setupMonsters();
    setupSkills();
    setupUI();

    this->scheduleUpdate();

    // Keyboard
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) m_keyUp = true;
        if (keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) m_keyDown = true;
        if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) m_keyLeft = true;
        if (keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) m_keyRight = true;
        if (keyCode == EventKeyboard::KeyCode::KEY_B) onToggleInventory();
        if (keyCode == EventKeyboard::KeyCode::KEY_C) onTogglePlayerInfo();
        if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
            auto player = Player::getInstance();
            if (player->isAlive()) performNormalAttack();
        }
        if (keyCode == EventKeyboard::KeyCode::KEY_1) onUseSkill(1);
        if (keyCode == EventKeyboard::KeyCode::KEY_2) onUseSkill(2);
        if (keyCode == EventKeyboard::KeyCode::KEY_3) onUseSkill(3);
    };
    keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) m_keyUp = false;
        if (keyCode == EventKeyboard::KeyCode::KEY_S || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) m_keyDown = false;
        if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) m_keyLeft = false;
        if (keyCode == EventKeyboard::KeyCode::KEY_D || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) m_keyRight = false;
    };

    // 窗口失焦再回来时重置所有按键状态
    auto focusListener = EventListenerCustom::create("GAME_RESTORE", [this](EventCustom*) {
        m_keyUp = false;
        m_keyDown = false;
        m_keyLeft = false;
        m_keyRight = false;
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(focusListener, this);

    // 监听技能栏点击事件
    auto skillEventListener = EventListenerCustom::create("skill_use", [this](EventCustom* event) {
        int skillId = static_cast<int>(reinterpret_cast<intptr_t>(event->getUserData()));
        onUseSkill(skillId);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(skillEventListener, this);

    return true;
}

void CityScene::setupMap() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    m_mapLayer = Sprite::create("ground.png");
    if (!m_mapLayer) {
        m_mapLayer = Sprite::create();
        m_mapLayer->setTextureRect(Rect(0, 0, 2400, 1800));
        m_mapLayer->setColor(Color3B(50, 60, 40));
    }
    m_mapLayer->setPosition(Vec2::ZERO);
    m_mapLayer->setAnchorPoint(Vec2::ZERO);
    this->addChild(m_mapLayer, -1);

    // Buildings (placeholder)
    auto buildTex = Director::getInstance()->getTextureCache()->addImage("building.png");
    if (buildTex) {
        for (int i = 0; i < 5; ++i) {
            auto building = Sprite::createWithTexture(buildTex);
            building->setPosition(Vec2(300 + rand() % 1800, 200 + rand() % 1400));
            building->setScale(0.8f + CCRANDOM_0_1() * 0.4f);
            this->addChild(building, 0);
        }
    }

    // NPCs
    auto npcIcon = Sprite::create("npc.png");
    if (npcIcon) {
        npcIcon->setPosition(Vec2(800, 600));
        this->addChild(npcIcon, 0);
        auto npcLabel = Label::createWithSystemFont("铁匠", "Arial", 14);
        npcLabel->setPosition(Vec2(800, 560));
        npcLabel->setTextColor(Color4B(255, 200, 100, 255));
        this->addChild(npcLabel, 0);
    }

    auto npcIcon2 = Sprite::create("npc.png");
    if (npcIcon2) {
        npcIcon2->setPosition(Vec2(1500, 900));
        this->addChild(npcIcon2, 0);
        auto npcLabel2 = Label::createWithSystemFont("药师", "Arial", 14);
        npcLabel2->setPosition(Vec2(1500, 860));
        npcLabel2->setTextColor(Color4B(100, 255, 100, 255));
        this->addChild(npcLabel2, 0);
    }
}

void CityScene::setupPlayer() {
    auto playerTex = Director::getInstance()->getTextureCache()->addImage("player.png");
    if (playerTex) {
        m_playerSprite = Sprite::createWithTexture(playerTex);
    } else {
        m_playerSprite = Sprite::create();
        m_playerSprite->setTextureRect(Rect(0, 0, 32, 48));
        m_playerSprite->setColor(Color3B(100, 180, 255));
    }
    m_playerSprite->setPosition(Vec2(640, 360));
    m_playerSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
    this->addChild(m_playerSprite, 2);

    auto nameLabel = Label::createWithSystemFont("玩家", "Arial", 12);
    nameLabel->setPosition(Vec2(0, 30));
    nameLabel->setTextColor(Color4B::WHITE);
    m_playerSprite->addChild(nameLabel);
}

void CityScene::setupMonsters() {
    const Vec2 spawnPositions[] = {
        Vec2(1100, 500), Vec2(1500, 300), Vec2(800, 1200),
        Vec2(1800, 1000), Vec2(500, 1400)
    };
    // 用 MonsterManager 生成，怪物类型多样
    int monsterTypes[] = {1001, 1002, 1003, 1004, 1005};
    for (int i = 0; i < 5; ++i) {
        MonsterManager::getInstance()->spawnMonsterInWorld(
            monsterTypes[i], spawnPositions[i], this);
    }
}

void CityScene::setupSkills() {
    auto player = Player::getInstance();

    Skill basicAttack;
    basicAttack.id = 1;
    basicAttack.name = "普攻";
    basicAttack.description = "基础近战攻击";
    basicAttack.cd = 0.5f;
    basicAttack.mpCost = 0;
    basicAttack.damageMultiplier = 1.0f;
    player->addSkill(basicAttack);

    Skill fireball;
    fireball.id = 2;
    fireball.name = "火球术";
    fireball.description = "发射一枚火球攻击敌人";
    fireball.cd = 3.0f;
    fireball.mpCost = 10;
    fireball.damageMultiplier = 1.8f;
    player->addSkill(fireball);

    Skill heal;
    heal.id = 3;
    heal.name = "治愈术";
    heal.description = "恢复自身生命值";
    heal.cd = 8.0f;
    heal.mpCost = 15;
    heal.damageMultiplier = 0.0f;
    player->addSkill(heal);
}

void CityScene::setupUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Joystick
    m_joystick = Joystick::create(100, Vec2(120, 120));
    if (m_joystick) {
        m_joystick->setPosition(Vec2(120, 120));
        this->addChild(m_joystick, 10);
    }

    // HP/MP Bar
    m_hpMpBar = HpMpBar::create(200, 16, Color3B::RED);
    if (m_hpMpBar) {
        m_hpMpBar->setPosition(Vec2(20, visibleSize.height - 50));
        this->addChild(m_hpMpBar, 10);
    }

    // Skill bar
    m_skillBar = SkillBar::create();
    if (m_skillBar) {
        m_skillBar->setPosition(Vec2(visibleSize.width / 2, 50));
        this->addChild(m_skillBar, 10);
    }

    // Attack button (右下)
    auto attackBtn = ui::Button::create("btn_normal.png", "btn_pressed.png");
    if (attackBtn) {
        attackBtn->setTitleText("攻击");
        attackBtn->setTitleFontSize(18);
        attackBtn->setPosition(Vec2(visibleSize.width - 120, 160));
        attackBtn->addClickEventListener([this](Ref*) {
            if (Player::getInstance()->isAlive()) performNormalAttack();
        });
        this->addChild(attackBtn, 10);
    }

    // Toggle buttons
    auto invBtn = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    if (invBtn) {
        invBtn->setTitleText("背包 (B)");
        invBtn->setTitleFontSize(14);
        invBtn->setPosition(Vec2(visibleSize.width - 100, 80));
        invBtn->addClickEventListener([this](Ref*) { onToggleInventory(); });
        this->addChild(invBtn, 10);
    }

    auto infoBtn = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    if (infoBtn) {
        infoBtn->setTitleText("属性 (C)");
        infoBtn->setTitleFontSize(14);
        infoBtn->setPosition(Vec2(visibleSize.width - 100, 40));
        infoBtn->addClickEventListener([this](Ref*) { onTogglePlayerInfo(); });
        this->addChild(infoBtn, 10);
    }

    // Panels
    m_inventoryPanel = InventoryPanel::create();
    if (m_inventoryPanel) {
        m_inventoryPanel->setVisible(false);
        this->addChild(m_inventoryPanel, 20);
    }

    m_playerInfoPanel = PlayerInfoPanel::create();
    if (m_playerInfoPanel) {
        m_playerInfoPanel->setVisible(false);
        this->addChild(m_playerInfoPanel, 20);
    }
}

void CityScene::update(float dt) {
    auto player = Player::getInstance();
    player->updateSkills(dt);

    // 键盘移动
    Vec2 moveDir = Vec2::ZERO;
    if (m_keyLeft) moveDir.x -= 1;
    if (m_keyRight) moveDir.x += 1;
    if (m_keyUp) moveDir.y += 1;
    if (m_keyDown) moveDir.y -= 1;

    // 摇杆移动（覆盖键盘方向）
    if (m_joystick) {
        auto joyDir = m_joystick->getDirection();
        if (joyDir.length() > 0.1f) {
            moveDir = joyDir;
        }
    }

    if (moveDir.length() > 0.1f) {
        moveDir.normalize();
        Vec2 move = moveDir * m_moveSpeed * dt;
        m_playerSprite->setPosition(m_playerSprite->getPosition() + move);

        auto pos = m_playerSprite->getPosition();
        pos.x = std::max(16.0f, std::min(2384.0f, pos.x));
        pos.y = std::max(16.0f, std::min(1784.0f, pos.y));
        m_playerSprite->setPosition(pos);

        // Camera
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto cameraPos = Vec2(
            std::max(visibleSize.width / 2, std::min(pos.x, 2400 - visibleSize.width / 2)),
            std::max(visibleSize.height / 2, std::min(pos.y, 1800 - visibleSize.height / 2))
        );
        this->setPosition(Vec2(visibleSize.width / 2 - cameraPos.x, visibleSize.height / 2 - cameraPos.y));

        // Flip
        m_playerSprite->setFlippedX(moveDir.x < 0);
    }

    // 更新 HP/MP bar
    m_hpMpBar->setHpRatio(static_cast<float>(player->getHp()) / player->getMaxHp());
    m_hpMpBar->setMpRatio(static_cast<float>(player->getMp()) / player->getMaxMp());

    // 更新怪物 AI（传递玩家位置）
    Vec2 playerPos = m_playerSprite->getPosition();
    MonsterManager::getInstance()->updateAllAI(dt, playerPos, player->isAlive());
    MonsterManager::getInstance()->removeDeadMonsters();

    // 攻击冷却
    if (m_attackCooldown > 0) m_attackCooldown -= dt;

    // 玩家死亡处理
    if (!player->isAlive()) {
        // 变灰
        m_playerSprite->setColor(Color3B(120, 120, 120));
    }

    // 刷新技能条
    m_skillBar->refresh();
}

// ===== 攻击/技能 =====

void CityScene::performNormalAttack() {
    auto player = Player::getInstance();
    if (m_attackCooldown > 0 || !player->canUseSkill(1)) return;
    m_attackCooldown = ATTACK_CD;
    player->useSkill(1);

    // 攻击动画
    if (m_playerSprite) {
        auto attackAnim = Sequence::create(
            ScaleTo::create(0.08f, 1.3f, 0.7f),
            ScaleTo::create(0.08f, 1.0f, 1.0f),
            nullptr
        );
        m_playerSprite->runAction(attackAnim);
    }

    // 找最近的目标
    Vec2 playerPos = m_playerSprite->getPosition();
    auto monsters = MonsterManager::getInstance()->getMonstersInRange(playerPos, 80.0f);
    for (auto* monster : monsters) {
        if (!monster || !monster->isAlive()) continue;
        int damage = player->getAttack();
        int def = monster->getDefense() / 2;
        int actualDmg = std::max(1, damage - def);

        // 找到这个怪物对应的 Sprite 位置用于显示伤害数字
        auto& instances = MonsterManager::getInstance()->getAllInstances();
        Vec2 monsterPos;
        for (auto& inst : instances) {
            if (inst.monsterData == monster && inst.sprite) {
                monsterPos = inst.sprite->getPosition();
                break;
            }
        }

        MonsterManager::getInstance()->damageMonster(monster, static_cast<float>(actualDmg), playerPos);
        spawnDamageNumber(monsterPos, actualDmg, Color4B(255, 200, 50, 255));
        break; // 只打一只
    }
}

void CityScene::performFireballSkill() {
    auto player = Player::getInstance();
    if (!player->canUseSkill(2)) return;
    player->useSkill(2);

    // 找最近的敌人发射火球
    Vec2 playerPos = m_playerSprite->getPosition();
    auto monsters = MonsterManager::getInstance()->getMonstersInRange(playerPos, 300.0f);
    if (monsters.empty()) return;

    Monster* target = monsters[0];
    Vec2 targetPos = Vec2::ZERO;
    auto& instances = MonsterManager::getInstance()->getAllInstances();
    for (auto& inst : instances) {
        if (inst.monsterData == target && inst.sprite) {
            targetPos = inst.sprite->getPosition();
            break;
        }
    }

    // 火球飞行动画
    auto fireball = Sprite::create();
    fireball->setTextureRect(Rect(0, 0, 16, 16));
    fireball->setColor(Color3B(255, 150, 50));
    fireball->setPosition(playerPos);
    this->addChild(fireball, 5);

    auto moveToTarget = MoveTo::create(0.3f, targetPos);
    auto explode = CallFunc::create([this, fireball, target, targetPos, playerPos]() {
        // 爆炸特效
        auto explosion = Sprite::create();
        explosion->setTextureRect(Rect(0, 0, 32, 32));
        explosion->setColor(Color3B(255, 100, 50));
        explosion->setPosition(targetPos);
        this->addChild(explosion, 5);

        auto expand = ScaleTo::create(0.15f, 2.0f);
        auto fadeOut = FadeOut::create(0.15f);
        auto remove = RemoveSelf::create();
        explosion->runAction(Sequence::create(Spawn::create(expand, fadeOut, nullptr), remove, nullptr));

        fireball->removeFromParent();

        // 伤害
        if (target && target->isAlive()) {
            auto player = Player::getInstance();
            int baseDmg = player->getAttack();
            int def = target->getDefense() / 2;
            int actualDmg = std::max(1, static_cast<int>(baseDmg * 1.8f) - def);
            MonsterManager::getInstance()->damageMonster(target, static_cast<float>(actualDmg), playerPos);
            spawnDamageNumber(targetPos, actualDmg, Color4B(255, 100, 50, 255));
        }
    });
    fireball->runAction(Sequence::create(moveToTarget, explode, nullptr));

    // 朝向目标
    m_playerSprite->setFlippedX(targetPos.x < playerPos.x);
}

void CityScene::performHealSkill() {
    auto player = Player::getInstance();
    if (!player->canUseSkill(3)) return;
    player->useSkill(3);

    int healAmount = player->getMaxHp() / 3;
    player->heal(healAmount);

    spawnDamageNumber(m_playerSprite->getPosition(), healAmount, Color4B(80, 255, 80, 255));

    // 绿色光效
    auto glow = Sprite::create();
    glow->setTextureRect(Rect(0, 0, 64, 64));
    glow->setColor(Color3B(80, 255, 80));
    glow->setOpacity(120);
    glow->setPosition(m_playerSprite->getPosition());
    this->addChild(glow, 5);

    auto expand = ScaleTo::create(0.4f, 2.0f);
    auto fadeOut = FadeOut::create(0.4f);
    auto remove = RemoveSelf::create();
    glow->runAction(Sequence::create(Spawn::create(expand, fadeOut, nullptr), remove, nullptr));
}

void CityScene::spawnDamageNumber(const Vec2& pos, int damage, const Color4B& color) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", damage);
    auto label = Label::createWithSystemFont(buf, "Arial", 18);
    label->setPosition(pos + Vec2(random(-15, 15), 10));
    label->setTextColor(color);
    this->addChild(label, 20);

    auto moveUp = MoveBy::create(0.6f, Vec2(0, 40));
    auto fadeOut = FadeOut::create(0.6f);
    auto remove = RemoveSelf::create();
    label->runAction(Sequence::create(Spawn::create(moveUp, fadeOut, nullptr), remove, nullptr));
}

// ===== 回调 =====

void CityScene::onAttack(const Vec2& direction) {
    performNormalAttack();
}

void CityScene::onUseSkill(int skillId) {
    if (skillId == 2) {
        performFireballSkill();
    } else if (skillId == 3) {
        performHealSkill();
    } else if (skillId == 1) {
        performNormalAttack();
    }
}

void CityScene::onToggleInventory() {
    m_inventoryVisible = !m_inventoryVisible;
    m_inventoryPanel->setVisible(m_inventoryVisible);
    if (m_inventoryVisible) m_inventoryPanel->refresh();
}

void CityScene::onTogglePlayerInfo() {
    m_playerInfoVisible = !m_playerInfoVisible;
    m_playerInfoPanel->setVisible(m_playerInfoVisible);
    if (m_playerInfoVisible) m_playerInfoPanel->refresh();
}
