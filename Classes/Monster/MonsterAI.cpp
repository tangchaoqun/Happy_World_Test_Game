#include "MonsterAI.h"
#include "Monster.h"
#include "../Player/Player.h"

USING_NS_CC;

MonsterAI::MonsterAI(Sprite* sprite)
    : _sprite(sprite)
    , _state(State::PATROL)
{
    if (_sprite) {
        _spawnPos = _sprite->getPosition();
        _currentHp = 100.0f;
        _maxHp = 100.0f;
        _patrolTarget = _spawnPos + Vec2(random(-80.0f, 80.0f), random(-80.0f, 80.0f));
    }
}

MonsterAI::~MonsterAI() {}

void MonsterAI::update(float dt, const Vec2& playerPos, bool playerAlive) {
    if (_state == State::DEAD || !_sprite) return;

    switch (_state) {
        case State::IDLE:
        case State::PATROL:
            updatePatrol(dt, playerPos, playerAlive);
            break;
        case State::CHASE:
            updateChase(dt, playerPos, playerAlive);
            break;
        case State::ATTACK:
            updateAttack(dt, playerPos, playerAlive);
            break;
        case State::RETURN:
            updateReturn(dt, playerPos, playerAlive);
            break;
        default:
            break;
    }
}

void MonsterAI::changeState(State newState) {
    _state = newState;
}

bool MonsterAI::isPlayerInRange(const Vec2& playerPos, float range) {
    if (!_sprite) return false;
    return _sprite->getPosition().distance(playerPos) <= range;
}

void MonsterAI::updatePatrol(float dt, const Vec2& playerPos, bool playerAlive) {
    if (!_sprite) return;

    if (isPlayerInRange(playerPos, _chaseRange)) {
        changeState(State::CHASE);
        return;
    }

    auto currentPos = _sprite->getPosition();
    auto distToTarget = currentPos.distance(_patrolTarget);

    if (distToTarget < 10.0f) {
        _patrolTimer += dt;
        if (_patrolTimer >= _patrolWaitTime) {
            _patrolTarget = _spawnPos + Vec2(
                random(-_patrolRange, _patrolRange),
                random(-_patrolRange, _patrolRange)
            );
            _patrolTimer = 0.0f;
        }
    } else {
        moveTo(_patrolTarget, 40.0f);
    }
}

void MonsterAI::updateChase(float dt, const Vec2& playerPos, bool playerAlive) {
    if (!playerAlive || !isPlayerInRange(playerPos, _chaseRange * 1.5f)) {
        changeState(State::RETURN);
        return;
    }
    if (isPlayerInRange(playerPos, _attackRange)) {
        changeState(State::ATTACK);
        return;
    }
    moveTo(playerPos, 80.0f);
}

void MonsterAI::updateAttack(float dt, const Vec2& playerPos, bool playerAlive) {
    if (!playerAlive || !isPlayerInRange(playerPos, _attackRange * 1.5f)) {
        changeState(State::CHASE);
        return;
    }

    _attackCooldown += dt;
    if (_attackCooldown >= _attackInterval) {
        // Deal damage to player
        auto player = Player::getInstance();
        player->takeDamage(10);
        _attackCooldown = 0.0f;
    }
}

void MonsterAI::updateReturn(float dt, const Vec2& playerPos, bool playerAlive) {
    if (!_sprite) return;

    if (playerAlive && isPlayerInRange(playerPos, _chaseRange)) {
        changeState(State::CHASE);
        return;
    }

    auto distToSpawn = _sprite->getPosition().distance(_spawnPos);
    if (distToSpawn < 10.0f) {
        _currentHp = _maxHp;
        changeState(State::PATROL);
    } else {
        moveTo(_spawnPos, 60.0f);
    }
}

void MonsterAI::moveTo(const Vec2& target, float speed) {
    if (!_sprite) return;
    auto currentPos = _sprite->getPosition();
    auto direction = (target - currentPos);
    float len = direction.length();
    if (len < 1.0f) return;

    direction = direction / len;
    float dt = 0.016f;
    auto newPos = currentPos + direction * speed * dt;
    _sprite->setPosition(newPos);

    _sprite->setFlippedX(direction.x < 0);
}

void MonsterAI::takeDamage(float damage, const Vec2& attackerPos) {
    if (_state == State::DEAD || !_sprite) return;

    _currentHp -= damage;
    if (_currentHp <= 0.0f) {
        _currentHp = 0.0f;
        onDeath();
    } else {
        auto dir = _sprite->getPosition() - attackerPos;
        _sprite->setFlippedX(dir.x < 0);

        if (_state != State::ATTACK && _state != State::CHASE) {
            changeState(State::CHASE);
        }
    }
}

void MonsterAI::onDeath() {
    changeState(State::DEAD);
    if (_sprite) {
        auto fadeOut = FadeOut::create(0.8f);
        auto scaleDown = ScaleTo::create(0.8f, 0.1f);
        auto spawn = Spawn::create(fadeOut, scaleDown, nullptr);
        auto callback = CallFunc::create([this]() {
            _readyToRemove = true;
        });
        _sprite->runAction(Sequence::create(spawn, callback, nullptr));
    } else {
        _readyToRemove = true;
    }
}

void MonsterAI::reset() {
    _currentHp = _maxHp;
    _state = State::PATROL;
    _patrolTimer = 0.0f;
    _attackCooldown = 0.0f;
    _readyToRemove = false;

    if (_sprite) {
        _sprite->setPosition(_spawnPos);
        _sprite->setVisible(true);
        _sprite->setOpacity(255);
        _sprite->setScale(1.0f);
    }
}
