#include "ui/CocosGUI.h"

#include "HelloMapleScene.h"
#include "SimpleAudioEngine.h"

#include "TextBox/TextBox.h"

#include "../BMGamingSDK/include/BMediaNativeCore.h"
#include "../BMGamingSDK/include/jsonRtc.h"


#include "MapleViewHelp.h"


USING_NS_CC;

using namespace maple;

static  BMediaKit* mBMedia = nullptr;
static std::string ownerUid = "";
bool exitFlag = false;

class MyIGamingRtcEngineEventHandler : public IBMediaEventHandler
{
private:
    HelloLanmo* mUi;

public:
    MyIGamingRtcEngineEventHandler(HelloLanmo* ui) :mUi(ui) {
    }

    ~MyIGamingRtcEngineEventHandler() {
    }

    void onJoinChannelSuccess(const char* channel, const char* uid) override
    {
        CCLOG("[General C++]:onJoinChannelSuccess, channel:%s, uid:%s", channel, uid);
        ownerUid = uid;
        std::stringstream rawMsg;
        rawMsg << "onJoinChannelSuccess, channel:" << channel << ", uid:" << uid;
        mUi->RenderLocalView();
        mUi->updateMsgContent(rawMsg.str());
    }

    void onLeaveChannel(int reason) override
    {
        CCLOG("[General C++]:onLeaveChannel, reason:%d ", reason);
        std::stringstream rawMsg;
        rawMsg << "onLeaveChannel, reason:" << reason;
        mUi->updateMsgContent(rawMsg.str());
        if(exitFlag) {
            exitFlag = false;
            // Close the cocos2d-x game scene and quit the application
            Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
        }

    }

    void onUserJoinedNotice(const char* uids) override {
        Json::Reader reader;
        Json::Value value;
        if(!reader.parse(uids, value)){
            return;
        }
        std::vector<std::string> new_uids;
        if(!rtc::JsonArrayToStringVector(value, &new_uids)){
            return;
        }

        for(int i = 0; i < new_uids.size(); i++){
            if(new_uids[i].find(ownerUid, 0) != std::string::npos)
                continue;
            mUi->RenderRemoteView(new_uids[i].c_str());
        }
        std::stringstream rawMsg;
        rawMsg << "onUserJoinedNotice, uids:"<<  uids;
        mUi->updateMsgContent(rawMsg.str());
    }

    void onUserOfflineNotice(const char* uids) override {
        
        Json::Reader reader;
        Json::Value value;
        if(!reader.parse(uids, value)){
            return;
        }
        std::vector<std::string> new_uids;
        if(!rtc::JsonArrayToStringVector(value, &new_uids)){
            return;
        }
        
        for(int i = 0; i < new_uids.size(); i++){
            if(new_uids[i].find(ownerUid, 0) != std::string::npos)
                continue;
            mUi->RenderRemoteView(new_uids[i].c_str());
        }
        
        std::stringstream rawMsg;
        rawMsg << "onUserOfflineNotice, uids:" << uids;
        mUi->updateMsgContent(rawMsg.str());
    }

    void onForceKickOutChannel(int reason) override{
        std::stringstream rawMsg;
        rawMsg << "onForceKickOutChannel, reason:" << reason;
        mUi->updateMsgContent(rawMsg.str());
    }

    void onAudioVolumeIndication(int volume) override{

    }

};

Scene* HelloLanmo::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloLanmo::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloLanmo::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloLanmo::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2 ,
                                origin.y + closeItem->getContentSize().height / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // create and initialize a label
    std::stringstream title;
    title << " BMGame ";
    title << BMediaKit_getInstance(MAPLE_APP_ID)->getSdkVersion();
    title << " DEMO";
    auto label = Label::createWithTTF(title.str().c_str(), "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    mMsgBox = TextBox::create("TextBox.png");
    mMsgBox->setPosition(Vec2(origin.x + visibleSize.width / 2 - 110,
                            origin.y + visibleSize.height - 210));

    mMsgBox->setSize(220, 160);

    this->addChild(mMsgBox, 0);

    int leftPadding = 10;

    mChannelEditBox = ui::EditBox::create(Size(120, 30), "TextBox.png");
    mChannelEditBox->setPlaceHolder("Enter Chan...");
    mChannelEditBox->setPosition(Vec2(origin.x + leftPadding + mChannelEditBox->getContentSize().width / 2, origin.y + visibleSize.height - 1.5 * mChannelEditBox->getContentSize().height));

    this->addChild(mChannelEditBox, 0);

    auto joinButton = ui::Button::create("Button.png", "ButtonPressed.png", "ButtonPressed.png");
    joinButton->setTitleText("Join Channel");
    joinButton->setPosition(Vec2(origin.x + leftPadding + joinButton->getContentSize().width / 2, origin.y + visibleSize.height - 1 * joinButton->getContentSize().height - 2 * mChannelEditBox->getContentSize().height));

    joinButton->addTouchEventListener([&](cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                onJoinChannelClicked();
                break;
            default:
                break;
        }
    });

    this->addChild(joinButton, 0);

    auto leaveButton = ui::Button::create("Button.png", "ButtonPressed.png", "ButtonPressed.png");
    leaveButton->setTitleText("Leave Channel");
    leaveButton->setPosition(Vec2(origin.x + leftPadding + leaveButton->getContentSize().width / 2, origin.y + visibleSize.height - 2 * leaveButton->getContentSize().height - 2 * mChannelEditBox->getContentSize().height));

    leaveButton->addTouchEventListener([&](cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                onLeaveChannelClicked();
                break;
            default:
                break;
        }
    });

    this->addChild(leaveButton, 0);


    auto openSpeakerphoneButton = ui::Button::create("Button.png", "ButtonPressed.png", "ButtonPressed.png");
    openSpeakerphoneButton->setTitleText("Open SpeakerPhone");
    openSpeakerphoneButton->setPosition(Vec2(origin.x + leftPadding + openSpeakerphoneButton->getContentSize().width / 2, origin.y + visibleSize.height - 3 * openSpeakerphoneButton->getContentSize().height - 2 * mChannelEditBox->getContentSize().height));

    openSpeakerphoneButton->addTouchEventListener([&](cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                onOpenSpeakerPhoneClicked();
                break;
            default:
                break;
        }
    });

    this->addChild(openSpeakerphoneButton, 0);

    auto closeSpeakerphoneButton = ui::Button::create("Button.png", "ButtonPressed.png", "ButtonPressed.png");
    closeSpeakerphoneButton->setTitleText("Close SpeakerPhone");
    closeSpeakerphoneButton->setPosition(Vec2(origin.x + leftPadding + closeSpeakerphoneButton->getContentSize().width / 2, origin.y + visibleSize.height - 4 * closeSpeakerphoneButton->getContentSize().height - 2 * mChannelEditBox->getContentSize().height));

    closeSpeakerphoneButton->addTouchEventListener([&](cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                onCloseSpeakerPhoneClicked();
                break;
            default:
                break;
        }
    });

    this->addChild(closeSpeakerphoneButton, 0);

    // add "HiLanmo" splash screen"
    auto sprite = Sprite::create("HelloCocos2dx.png");

    sprite->setPosition(Vec2(origin.x + visibleSize.width - sprite->getContentSize().width / 2 ,
                                visibleSize.height / 2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    mBMedia = BMediaKit_getInstance(MAPLE_APP_ID);

    if(mBMedia){
        mBMedia->setEventHandler(new MyIGamingRtcEngineEventHandler(this));

        mBMedia->setChannelProfile(CHANNEL_PROFILE_VOICE_VIDEO);

        int version = mBMedia->getSdkVersion();
    }


    scheduleUpdate();

    return true;
}

void HelloLanmo::update(float delta)
{
}

void HelloLanmo::updateMsgContent(const std::string& msg)
{
    mMsgBox->setText(msg);
}

void HelloLanmo::onLeaveChannelClicked()
{
    BMediaKit_getInstance(MAPLE_APP_ID)->leaveChannel();
}

void HelloLanmo::onJoinChannelClicked()
{
    if (mChannelEditBox == nullptr || ::strlen(mChannelEditBox->getText()) == 0) {
        return;
    }

    auto rtcEngine = BMediaKit_getInstance(MAPLE_APP_ID);
    //rtcEngine->joinChannel(mChannelEditBox->getText(), "Cocos2d");
    rtcEngine->joinChannel(mChannelEditBox->getText());
}

void HelloLanmo::onOpenSpeakerPhoneClicked()
{
    int ret = BMediaKit_getInstance(MAPLE_APP_ID)->setEnableSpeakerphone(true);

    std::stringstream rawMsg;
    rawMsg << "setEnableSpeakerphone true, ret = " << ret;
    this->updateMsgContent(rawMsg.str());
}

void HelloLanmo::onCloseSpeakerPhoneClicked()
{
    int ret = BMediaKit_getInstance(MAPLE_APP_ID)->setEnableSpeakerphone(false);
    std::stringstream rawMsg;
    rawMsg << "setEnableSpeakerphone false, ret = " << ret;
    this->updateMsgContent(rawMsg.str());
}

void HelloLanmo::menuCloseCallback(Ref* pSender)
{
    onLeaveChannelClicked();

    exitFlag = true;



    // Close the cocos2d-x game scene and quit the application
    /*Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
     */

    /*To navigate back to native iOS screen(if present) without quitting the application, do not use Director::getInstance()->end() and exit(0) as given above, instead trigger a custom event created in RootViewController.mm as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    // _eventDispatcher->dispatchEvent(&customEndEvent);
}


void HelloLanmo::RenderLocalView()
{
    local_view_= mBMedia->createRenderView();
    MapleViewHelp::SetSurfaceView(local_view_, remotePosition[0].x, remotePosition[0].y);
    BMVideoCanvas vc;
    vc.view = local_view_;
    mBMedia->setupLocalVideo(vc);
    mBMedia->startPreview();
    mBMedia->muteLocalVideoStream(false);
}

void HelloLanmo::RemoveLocalView()
{
    if(local_view_){
        int ret =  mBMedia->muteLocalVideoStream(true);
        mBMedia->stopPreview();
        BMVideoCanvas vc;
        vc.view = nullptr;
        mBMedia->setupLocalVideo(vc);
        MapleViewHelp::RemoveSurfaceView(local_view_);
        local_view_ = nullptr;
    }
}

void HelloLanmo::RenderRemoteView(const char* uid)
{
    //support max 4 users
    if(remote_views_.size() > 4){
        return;
    }

    auto it = remote_views_.find(uid);
    if (it != remote_views_.end()) {
        return;
    }

    std::string uid_ = uid;

    /*for(int i = 0; i < 4; i ++){
        if(uid_.find(remotePosition[i].name, 0) != std::string::npos)
            return;
    }*/

    remote_views_[uid] = mBMedia->createRenderView();
    MapleViewHelp::SetSurfaceView(remote_views_[uid], remotePosition[1].x, remotePosition[1].y);
    BMVideoCanvas vc;
    vc.view = remote_views_[uid];
    vc.uid = (char*) uid;
    mBMedia->setupRemoteVideo(vc);
    mBMedia->muteRemoteVideoStream(uid, false);
}


void HelloLanmo::RemoveRemoteView(const char* uid)
{
    auto it = remote_views_.find(uid);
    if (it != remote_views_.end()) {
        //停止接受远端视频
        int ret =  mBMedia->muteRemoteVideoStream(uid, true);
        //解绑远端视频view
        BMVideoCanvas vc;
        vc.view = nullptr;
        vc.uid = (char*) uid;
        mBMedia->setupRemoteVideo(vc);
        MapleViewHelp::RemoveSurfaceView(remote_views_[uid]);
        remote_views_.erase(uid);
    }
}
