#ifndef __HELLOLANMO_SCENE_H__
#define __HELLOLANMO_SCENE_H__

#include "cocos2d.h"

#include "ui/UIEditBox/UIEditBox.h"

// PLEASE KEEP THIS App ID IN SAFE PLACE -->
// Get your own App ID at qq群 701150764
// After you entered the App ID, remove <##> outside of Your App ID

#define MAPLE_APP_ID <##>

USING_NS_CC;

class TextBox;
const struct
{
    char *name;
    const int x;
    const int y;
}remotePosition[] =
{
        {"",  2,  2},
        {"",  3,  2},
        {"",  2,  3},
        {"",  3,  3}
};

class HelloLanmo : public cocos2d::Layer
{

private:
    TextBox *mMsgBox = nullptr;
    ui::EditBox *mChannelEditBox = nullptr;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    virtual void update(float delta);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloLanmo);

private:
    void onJoinChannelClicked();
    void onLeaveChannelClicked();

    void onOpenSpeakerPhoneClicked();
    void onCloseSpeakerPhoneClicked();

public:
    void updateMsgContent(const std::string& msg);

    void RenderLocalView();
    void RemoveLocalView();
    void RenderRemoteView(const char* uid);
    void RemoveRemoteView(const char* uid);


private:
    void * local_view_;
    std::map<std::string, void*>   remote_views_;

};

#endif
