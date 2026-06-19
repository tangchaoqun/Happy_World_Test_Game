#include "LoginScene.h"
#include "CityScene.h"
#include "../Manager/GameDataManager.h"
#include "../Network/NetworkManager.h"
#include "../Player/Player.h"

#include "2d/CCTransition.h"
#include "platform/desktop/CCGLViewImpl-desktop.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include <windows.h>
#endif

USING_NS_CC;

Scene* LoginScene::createScene() {
    return LoginScene::create();
}

bool LoginScene::init() {
    if (!Scene::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float centerX = visibleSize.width / 2;

    // Background
    auto bg = LayerColor::create(Color4B(20, 24, 36, 255));
    this->addChild(bg);

    // Title
    auto title = Label::createWithSystemFont("盛 世 天 下", "Arial", 48);
    title->setPosition(Vec2(centerX, visibleSize.height * 0.75f));
    title->setTextColor(Color4B::WHITE);
    this->addChild(title);

    // Subtitle
    auto subtitle = Label::createWithSystemFont("Cocos2d-x C++ Edition", "Arial", 18);
    subtitle->setPosition(Vec2(centerX, visibleSize.height * 0.68f));
    subtitle->setTextColor(Color4B(150, 150, 180, 255));
    this->addChild(subtitle);

    float inputWidth = 320;
    float inputHeight = 44;

    // Account input background
    auto accBg = ui::Scale9Sprite::create();
    accBg->setContentSize(Size(inputWidth, inputHeight));
    accBg->setPosition(Vec2(centerX, visibleSize.height * 0.50f));
    accBg->setColor(Color3B(40, 45, 60));
    this->addChild(accBg);

    // Account placeholder label
    m_accountLabel = Label::createWithSystemFont("输入账号", "Arial", 20);
    m_accountLabel->setPosition(Vec2(centerX, visibleSize.height * 0.50f));
    m_accountLabel->setTextColor(Color4B(150, 150, 180, 255));
    this->addChild(m_accountLabel);

    // Password input background
    auto pwBg = ui::Scale9Sprite::create();
    pwBg->setContentSize(Size(inputWidth, inputHeight));
    pwBg->setPosition(Vec2(centerX, visibleSize.height * 0.42f));
    pwBg->setColor(Color3B(40, 45, 60));
    this->addChild(pwBg);

    // Password placeholder label
    m_passwordLabel = Label::createWithSystemFont("输入密码", "Arial", 20);
    m_passwordLabel->setPosition(Vec2(centerX, visibleSize.height * 0.42f));
    m_passwordLabel->setTextColor(Color4B(150, 150, 180, 255));
    this->addChild(m_passwordLabel);

    // Login button
    float btnWidth = 280;
    float btnHeight = 48;

    auto loginBtn = ui::Button::create();
    loginBtn->setContentSize(Size(btnWidth, btnHeight));
    auto loginNormal = ui::Scale9Sprite::create();
    loginNormal->setContentSize(Size(btnWidth, btnHeight));
    loginNormal->setColor(Color3B(60, 100, 200));
    loginNormal->setOpacity(230);
    loginBtn->addChild(loginNormal, -1);
    auto loginPressed = ui::Scale9Sprite::create();
    loginPressed->setContentSize(Size(btnWidth, btnHeight));
    loginPressed->setColor(Color3B(40, 70, 160));
    loginPressed->setOpacity(255);
    loginPressed->setVisible(false);
    loginBtn->addChild(loginPressed, -1);

    loginBtn->setTitleText("登  录");
    loginBtn->setTitleFontSize(22);
    loginBtn->setTitleColor(Color3B::WHITE);
    loginBtn->setPosition(Vec2(centerX, visibleSize.height * 0.32f));
    loginBtn->addClickEventListener(CC_CALLBACK_1(LoginScene::onLogin, this));
    this->addChild(loginBtn);

    // Register button
    auto regBtn = ui::Button::create();
    regBtn->setContentSize(Size(btnWidth, btnHeight));
    auto regNormal = ui::Scale9Sprite::create();
    regNormal->setContentSize(Size(btnWidth, btnHeight));
    regNormal->setColor(Color3B(50, 55, 75));
    regNormal->setOpacity(220);
    regBtn->addChild(regNormal, -1);
    auto regPressed = ui::Scale9Sprite::create();
    regPressed->setContentSize(Size(btnWidth, btnHeight));
    regPressed->setColor(Color3B(35, 40, 55));
    regPressed->setOpacity(255);
    regPressed->setVisible(false);
    regBtn->addChild(regPressed, -1);

    regBtn->setTitleText("注册账号");
    regBtn->setTitleFontSize(22);
    regBtn->setTitleColor(Color3B::WHITE);
    regBtn->setPosition(Vec2(centerX, visibleSize.height * 0.26f));
    regBtn->addClickEventListener(CC_CALLBACK_1(LoginScene::onRegister, this));
    this->addChild(regBtn);

    // Status label
    m_statusLabel = Label::createWithSystemFont("", "Arial", 15);
    m_statusLabel->setPosition(Vec2(centerX, visibleSize.height * 0.20f));
    m_statusLabel->setTextColor(Color4B(255, 80, 80, 255));
    this->addChild(m_statusLabel);

    // Version info
    auto version = Label::createWithSystemFont("v1.0.0", "Arial", 12);
    version->setPosition(Vec2(visibleSize.width - 60, 20));
    version->setTextColor(Color4B(80, 80, 100, 255));
    this->addChild(version);

    // 跳过登录按钮
    auto skipBtn = ui::Button::create();
    skipBtn->setContentSize(Size(120, 36));
    auto skipNormal = ui::Scale9Sprite::create();
    skipNormal->setContentSize(Size(120, 36));
    skipNormal->setColor(Color3B(80, 80, 100));
    skipNormal->setOpacity(180);
    skipBtn->addChild(skipNormal, -1);
    skipBtn->setTitleText("跳过登录");
    skipBtn->setTitleFontSize(16);
    skipBtn->setTitleColor(Color3B::WHITE);
    skipBtn->setPosition(Vec2(visibleSize.width - 80, 60));
    skipBtn->addClickEventListener([this](Ref*) {
        this->skipLogin();
    });
    this->addChild(skipBtn);

    return true;
}

void LoginScene::onEnter() {
    Scene::onEnter();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    auto glview = Director::getInstance()->getOpenGLView();
    if (!glview) return;

    m_hwndParent = glview->getWin32Window();
    if (!m_hwndParent) return;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    float centerX = visibleSize.width / 2;

    // 创建账号输入框
    createWin32Edit(1,
        (int)(centerX - 160), (int)(visibleSize.height * 0.50f - 22),
        320, 44, false);

    // 创建密码输入框
    createWin32Edit(2,
        (int)(centerX - 160), (int)(visibleSize.height * 0.42f - 22),
        320, 44, true);

    // 设置输入框文字颜色为白色
    if (m_hwndAccount) {
        SetTextColor(GetDC(m_hwndAccount), RGB(255, 255, 255));
        SetBkColor(GetDC(m_hwndAccount), RGB(40, 45, 60));
    }
    if (m_hwndPassword) {
        SetTextColor(GetDC(m_hwndPassword), RGB(255, 255, 255));
        SetBkColor(GetDC(m_hwndPassword), RGB(40, 45, 60));
    }

    // 确保输入框在前景并可以接收点击
    ShowWindow(m_hwndAccount, SW_SHOW);
    ShowWindow(m_hwndPassword, SW_SHOW);
    BringWindowToTop(m_hwndAccount);
    BringWindowToTop(m_hwndPassword);
#endif
}

void LoginScene::onExit() {
    destroyWin32Edits();
    Scene::onExit();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void LoginScene::createWin32Edit(int id, float x, float y, float w, float h, bool isPassword) {
    if (!m_hwndParent) return;

    // 转换cocos坐标到Win32窗口坐标（Y轴翻转）
    RECT rc;
    GetClientRect(m_hwndParent, &rc);
    int winY = (int)(rc.bottom - y - h);
    int winX = (int)x;
    int winW = (int)w;
    int winH = (int)h;

    DWORD style = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL;
    if (isPassword) style |= ES_PASSWORD;

    HWND hwnd = CreateWindowExW(
        WS_EX_CLIENTEDGE,
        L"EDIT",
        L"",
        style,
        winX, winY, winW, winH,
        m_hwndParent,
        (HMENU)(size_t)id,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (hwnd) {
        // 设置字体
        HFONT hFont = CreateFontW(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
        SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);

        // 设置文字颜色为白色 - 使用 WM_CTLCOLOREDIT 消息处理
        // EM_SETBKGNDCOLOR 需要 RichEdit，普通 Edit 控件不支持
        
        // 保存句柄
        if (id == 1) m_hwndAccount = hwnd;
        else m_hwndPassword = hwnd;
    }
}

void LoginScene::destroyWin32Edits() {
    if (m_hwndAccount) {
        DestroyWindow(m_hwndAccount);
        m_hwndAccount = nullptr;
    }
    if (m_hwndPassword) {
        DestroyWindow(m_hwndPassword);
        m_hwndPassword = nullptr;
    }
    m_hwndParent = nullptr;
}

std::string LoginScene::getWin32EditText(int id) {
    HWND hwnd = (id == 1) ? m_hwndAccount : m_hwndPassword;
    if (!hwnd) return "";

    int len = GetWindowTextLengthW(hwnd);
    if (len == 0) return "";

    std::wstring wstr(len + 1, 0);
    GetWindowTextW(hwnd, &wstr[0], len + 1);

    // wchar_t to UTF-8
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(utf8Len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], utf8Len, nullptr, nullptr);
    return result;
}

void LoginScene::setWin32EditText(int id, const std::string& text) {
    HWND hwnd = (id == 1) ? m_hwndAccount : m_hwndPassword;
    if (!hwnd) return;

    int wlen = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
    std::wstring wstr(wlen, 0);
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &wstr[0], wlen);

    SetWindowTextW(hwnd, wstr.c_str());
}

void LoginScene::showWin32Edits(bool show) {
    if (m_hwndAccount) ShowWindow(m_hwndAccount, show ? SW_SHOW : SW_HIDE);
    if (m_hwndPassword) ShowWindow(m_hwndPassword, show ? SW_SHOW : SW_HIDE);
}
#endif

void LoginScene::onLogin(Ref* sender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::string account = getWin32EditText(1);
    std::string password = getWin32EditText(2);
#else
    std::string account = "";
    std::string password = "";
#endif

    if (account.empty() || password.empty()) {
        m_statusLabel->setString("请输入账号和密码");
        return;
    }

    m_statusLabel->setString("登录中...");
    m_statusLabel->setTextColor(Color4B(180, 180, 200, 255));

    bool success = false;
    std::string msg;

    if (account == "admin" && password == "123") {
        success = true;
        msg = "管理员登录成功";
    } else {
        auto savedPwd = GameDataManager::getInstance()->getConfig("user_" + account, "");
        if (savedPwd.empty()) {
            msg = "账号不存在，请先注册";
        } else if (savedPwd != password) {
            msg = "密码错误";
        } else {
            success = true;
            msg = "登录成功";
        }
    }

    if (success) {
        auto player = Player::getInstance();
        player->setPlayerName(account);
        if (account == "admin") {
            player->setLevel(50);
        }
    }

    onLoginResult(success, msg);
}

void LoginScene::onRegister(Ref* sender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    std::string account = getWin32EditText(1);
    std::string password = getWin32EditText(2);
#else
    std::string account = "";
    std::string password = "";
#endif

    if (account.empty() || password.empty()) {
        m_statusLabel->setString("请输入账号和密码");
        return;
    }

    if (account == "admin") {
        m_statusLabel->setString("不能注册管理员账号");
        m_statusLabel->setTextColor(Color4B(255, 80, 80, 255));
        return;
    }

    auto savedPwd = GameDataManager::getInstance()->getConfig("user_" + account, "");
    if (!savedPwd.empty()) {
        m_statusLabel->setString("账号已存在，请直接登录");
        m_statusLabel->setTextColor(Color4B(255, 180, 80, 255));
        return;
    }

    GameDataManager::getInstance()->setConfig("user_" + account, password);
    GameDataManager::getInstance()->saveConfig();

    m_statusLabel->setTextColor(Color4B(80, 200, 120, 255));
    m_statusLabel->setString("注册成功，请登录");
}

void LoginScene::onLoginResult(bool success, const std::string& msg) {
    if (success) {
        m_statusLabel->setTextColor(Color4B(80, 200, 120, 255));
        m_statusLabel->setString(msg);

        auto delay = DelayTime::create(0.5f);
        auto callback = CallFunc::create([this]() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            showWin32Edits(false);
#endif
            auto cityScene = CityScene::createScene();
            auto transition = TransitionFade::create(0.5f, cityScene, Color3B::BLACK);
            Director::getInstance()->replaceScene(transition);
        });
        this->runAction(Sequence::create(delay, callback, nullptr));
    } else {
        m_statusLabel->setTextColor(Color4B(255, 80, 80, 255));
        m_statusLabel->setString(msg);
    }
}

void LoginScene::skipLogin() {
    // 自动以默认用户进入游戏
    auto player = Player::getInstance();
    player->setPlayerName("侠客");
    player->setLevel(1);

    // 不隐藏 win32 edit，让 onExit 自动清理
    auto cityScene = CityScene::createScene();
    auto transition = TransitionFade::create(0.5f, cityScene, Color3B::BLACK);
    Director::getInstance()->replaceScene(transition);
}
