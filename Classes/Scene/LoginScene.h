#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    #include <windows.h>
#endif

class LoginScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(LoginScene);

private:
    void onLogin(cocos2d::Ref* sender);
    void onRegister(cocos2d::Ref* sender);
    void onLoginResult(bool success, const std::string& msg);
    void skipLogin();

    void createWin32Edit(int id, float x, float y, float w, float h, bool isPassword);
    void destroyWin32Edits();
    std::string getWin32EditText(int id);
    void setWin32EditText(int id, const std::string& text);
    void showWin32Edits(bool show);

    cocos2d::Label* m_accountLabel = nullptr;
    cocos2d::Label* m_passwordLabel = nullptr;
    cocos2d::Label* m_statusLabel = nullptr;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    HWND m_hwndAccount = nullptr;
    HWND m_hwndPassword = nullptr;
    HWND m_hwndParent = nullptr;
#endif
};

#endif // __LOGIN_SCENE_H__
