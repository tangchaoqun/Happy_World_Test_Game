#ifndef __MONSTER_AI_H__
#define __MONSTER_AI_H__

#include "cocos2d.h"

class Monster;

class MonsterAI : public cocos2d::Ref {
public:
    enum class State {
        IDLE,
        PATROL,
        CHASE,
        ATTACK,
        RETURN,
        DEAD
    };

    MonsterAI(cocos2d::Sprite* sprite);
    virtual ~MonsterAI();

    void update(float dt, const cocos2d::Vec2& playerPos, bool playerAlive);
    void takeDamage(float damage, const cocos2d::Vec2& attackerPos);
    void onDeath();
    void reset();
    void setSpawnPosition(const cocos2d::Vec2& pos) { _spawnPos = pos; }

    bool isDead() const { return _state == State::DEAD; }
    bool isReadyToRemove() const { return _readyToRemove; }
    State getState() const { return _state; }
    float getCurrentHp() const { return _currentHp; }
    float getMaxHp() const { return _maxHp; }
    Monster* getMonster() const { return nullptr; }

private:
    cocos2d::Sprite* _sprite;
    State _state;

    cocos2d::Vec2 _spawnPos;
    float _patrolRange = 100.0f;
    float _chaseRange = 250.0f;
    float _attackRange = 60.0f;

    float _attackCooldown = 0.0f;
    float _attackInterval = 1.5f;
    float _patrolTimer = 0.0f;
    float _patrolWaitTime = 2.0f;

    float _currentHp = 0.0f;
    float _maxHp = 0.0f;

    cocos2d::Vec2 _patrolTarget;
    bool _readyToRemove = false;

    void changeState(State newState);
    void updatePatrol(float dt, const cocos2d::Vec2& playerPos, bool playerAlive);
    void updateChase(float dt, const cocos2d::Vec2& playerPos, bool playerAlive);
    void updateAttack(float dt, const cocos2d::Vec2& playerPos, bool playerAlive);
    void updateReturn(float dt, const cocos2d::Vec2& playerPos, bool playerAlive);

    void moveTo(const cocos2d::Vec2& target, float speed);
    bool isPlayerInRange(const cocos2d::Vec2& playerPos, float range);
};

#endif // __MONSTER_AI_H__
