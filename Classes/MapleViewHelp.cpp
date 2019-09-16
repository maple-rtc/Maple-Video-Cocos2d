#include "cocos2d.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "MapleViewHelp.h"

#include <android/log.h>
#define  LOG_TAG    "MapleViewHelp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)


#define CLASS_NAME "org/cocos2dx/cpp/MapleViewHelp"

using namespace cocos2d;


void MapleViewHelp::RemoveSurfaceView(void* view){
    JniMethodInfo t;
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "RemoveSurfaceView", "(Landroid/view/SurfaceView;)V");
    LOGD("RemoveRemoteSurfaceView, ret:%d",ret);
    if(ret)
    {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, reinterpret_cast<jobject>(view));
    }
}

void MapleViewHelp::SetSurfaceView(void* view, int x, int y)
{
    JniMethodInfo t;
    bool ret = cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "SetSurfaceView", "(Landroid/view/SurfaceView;II)V");
    LOGD("setLocalSurfaceView, ret:%d",ret);
    if(ret) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, reinterpret_cast<jobject>(view), x, y);
    }
 }

