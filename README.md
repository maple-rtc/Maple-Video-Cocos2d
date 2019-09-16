# Maple-Video-Cocos2d
## 简介：
maple-rtc 为蓝蘑云推出的实时音视频系统，包括了实时音频，视频，变声，美颜等功能，适用于娱乐，游戏，教育等实时场景中；

----------
- 完整的 API 文档和集成步骤见 [文档中心](http://doc.lmaple.com/maple-rtc-cocos2dx-voice-sdk.html)

----------
这个开源项目演示了如何快速集成 maple-rtc SDK 到 Cocos2d 中，实现在游戏中音视频通话效果。

在这个示例项目中包含了以下功能：
- 加入通话和离开通话；
- 扬声器和听筒切换功能；
- 显示本地视频预览；
- 显示远端视频预览

## 运行示例程序
首先在 联系对接群 **701150764** 注册账号，获取到 AppID。将 AppID 填写进 "Classes/HelloMapleScene.h"

```
#define MAPLE_APP_ID <#YOUR APP ID#>
```

然后在 [maple-rtc SDK](http://sdk.lmaple.com/Maple_Cocos2d_SDK_Release.zip) 下载 **maple-rtc 游戏语音 SDK for Cocos2d**，解压后将其中的 **include** 和 **libs** 文件夹复制到本项目的 **BMGamingSDK** 下。

- 本示例项目依赖于开源的 [Cocos2d-x](http://www.cocos2d-x.org/) 项目，下载 http://www.cocos2d-x.org/filedown/cocos2d-x-3.14.1.zip 并用相应内容来替换本项目下的 **cocos2d** 文件夹
- maple-rtc 游戏 SDK 本身并不依赖于特定的 Cocos2d 的版本；

- **运行 Android 项目**

使用 Android Studio 打开该项目，连上设备，编译并运行。

- **运行 iOS 项目**

使用 Xcode 打开项目直接编译运行。

## 运行环境
- Android Studio 2.0+ 或 Xcode 8.0+
- 真实 Android / iOS 设备
- SDK不包含模拟器库，所以推荐使用真机调试运行

## 联系我们

对接qq群  **701150764**