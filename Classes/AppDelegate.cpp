#include "AppDelegate.h"
#include "Scene/LoginScene.h"
#include "Scene/CityScene.h"
#include "Manager/GameDataManager.h"
#include "Network/NetworkManager.h"
#include "Player/Player.h"

USING_NS_CC;

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("HappyWorld", cocos2d::Rect(0, 0, 1280, 720));
#else
        glview = GLViewImpl::create("HappyWorld");
#endif
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    // Initialize managers
    GameDataManager::getInstance()->initialize();

    // 初始化默认玩家数据，跳过登录
    auto player = Player::getInstance();
    player->setPlayerName("侠客");
    player->setLevel(1);

    // 直接进入主城场景
    director->runWithScene(CityScene::createScene());

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    // 窗口恢复时发送事件，让场景重置按键状态
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("GAME_RESTORE");
}
