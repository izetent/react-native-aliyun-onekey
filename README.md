
### react-native-ali-onepass
  此项目由[react-native-ali-onepass](https://github.com/he-yf/react-native-ali-onepass) fork 而来

#### 安装

`$ npm install react-native-ali-one-pass --save`

`$ yarn add react-native-ali-one-pass`

对于android端需在在  `android/app/build.gradle`中添加下面的代码
```
android {
  repositories {
      flatDir {
          dirs 'libs', '../../node_modules/react-native-ali-one-pass/android/libs'
      }
  }
}
```

对于ios还需要运行以下命令
`$ cd ios && pod install`

#### api说明
详细内容请看[api详细说明](./index.d.ts)

#### 用法

使用示例：封装成hook使用
```js
import {useFocusEffect} from '@react-navigation/native';
import {useCallback} from 'react';
import {useRef} from 'react';
import {Platform} from 'react-native';
import * as onePassSDK from 'react-native-ali-one-pass';
import {useDispatch} from 'react-redux';

export async function setUIConfig() {
  const operatorType = await onePassSDK.getOperatorType();
  return onePassSDK.setUIConfig({
    //状态栏
    lightColor: true,
    statusBarHidden: true,
    // 弹窗使用
    dialogHeightDelta: 100,
    alertBarHidden: true,
    // alertBarCloseImgPath: 'onepass_close_btn',
    alertBarCloseImgWidth: 30,
    alertBarCloseImgHeight: 30,
    // 标题栏
    navColor: '#FFFFFF',
    navTextColor: '#333333',
    navText: '',
    navTextSize: 18,
    webNavColor: '#FFFFFF',
    webNavTextColor: '#333333',
    webNavTextSize: 18,
    navReturnImgPath: 'back_icon',
    navHidden: true,
    // logo
    logoImgPath: 'ic_launcher_round',
    logoHidden: false,
    logoWidth: 80,
    logoHeight: 80,
    // logoOffsetY: getOffsetY(175),
    // 手机号掩码
    numberColor: Platform.OS === 'android' ? '#f5f5f5' : '#333333',
    numberSize: 25,
    // slogan
    sloganText: `${operatorType}提供认证服务`,
    sloganTextColor: '#B2B2B2',
    sloganTextSize: 13,
    // sloganOffsetY: getOffsetY(384),
    // 登录按钮
    logBtnText: '本机号码一键登录',
    logBtnTextColor: '#f5f5f5',
    logBtnTextSize: 18,
    // ios
    // logBtnBackgroundPaths: ['loginbg', 'loginbg', 'loginbg'],
    // android
    // logBtnBackgroundPath: 'loginbg',
    logBtnMarginLeftAndRight: 35,
    // 其他登录方式
    switchAccText: '其他登录方式',
    switchAccTextSize: 15,
    switchAccTextColor: '#0EA8FF',
    // 协议栏
    privacyBefore: '登录即同意',
    privacyEnd: '并授权获取本机号码',
    checkboxHidden: true,
    privacyState: true,
    appPrivacyOneName: '协议',
    appPrivacyOneUrl: 'https://www.baidu.com',
    vendorPrivacyPrefix: '《',
    vendorPrivacySuffix: '》',
    privacyTextSize: 12,
    appPrivacyBaseColor: '#B2B2B2',
    appPrivacyColor: '#0EA8FF',
    pageBackgroundPath: 'loginbg',
  });
}

const androidAuthKey = '**********';
const iosAuthKey = '**************';

/**
 * 手机号一键登录hook
 */
const usePhoneLogin = () => {
  const dispatch = useDispatch();

  // 是否允许一键登录
  const onePassEnable = useRef(false);
  // 一键登录API初始化状态
  const onePassSDKIniting = useRef(true);
  // 延迟执行函数队列
  const delayFuncQueue = useRef<Function[]>([]);

  // 调用延迟函数执行队列
  const callDelayFuncQueue = useCallback(() => {
    // 执行初始化前调用的函数
    // 为了防止延迟执行的函数与初始化状态海边函数同时执行，推迟函数队列的执行时间
    setTimeout(() => {
      delayFuncQueue.current.forEach(func => {
        func();
      });
      delayFuncQueue.current = [];
    }, 100);
  }, []);

  useFocusEffect(
    useCallback(() => {
      // 初始化
      onePassSDK
        .init(Platform.OS === 'android' ? androidAuthKey : iosAuthKey)
        .then(async () => {
          // 检测一键登录是否可用
          onePassSDK
            .checkEnvAvailable(2)
            .then(async () => {
              // 预获取手机号
              onePassSDK
                .prefetch()
                .then(() => {
                  onePassEnable.current = true;
                })
                .catch(res => {
                  console.log(res.code, 'presetch');
                })
                .finally(() => {
                  // 设置SDK加载状态
                  onePassSDKIniting.current = false;
                  callDelayFuncQueue();
                });
            })
            .catch(res => {
              // 设置SDK加载状态
              onePassSDKIniting.current = false;
              callDelayFuncQueue();
            });
        })
        .catch(res => {
          // 设置SDK加载状态
          onePassSDKIniting.current = false;
          callDelayFuncQueue();
        });
      const succListener = onePassSDK.addListener(
        onePassSDK.EVENTS.onTokenSuccess,
        (data: any) => {
          // 运营商Token获取成功
          if (data.code === onePassSDK.RESULT_CODES.TOKENSUCCESS) {
            // 获取token成功，将token传递后端校验
            console.log(data.token);
          } else {
            if (
              ![onePassSDK.RESULT_CODES.AUTHPAGESUCCESS, '700004'].includes(
                data.code,
              )
            ) {
              onePassSDK.hideLoginLoading();
              onePassSDK.quitLoginPage();
              console.log('popuplogin');
            }
          }
        },
      );
      const failListener = onePassSDK.addListener(
        onePassSDK.EVENTS.onTokenFailed,
        (data: {code: string; msg: string}) => {
          console.log(data.code, onePassSDK.RESULT_CODES.SWITCHAUTHWAY);
          if (data.code === onePassSDK.RESULT_CODES.SWITCHAUTHWAY) {
            if (Platform.OS === 'android') {
              console.log('正在切换登录方式','android跳转');
            } else {
              console.log('正在切换登录方式','ios跳转');
            }
            setTimeout(() => {
              onePassSDK.quitLoginPage();
            }, 300);
          }
          console.log(data, 'fail data 一键登录失败');
          onePassSDK.hideLoginLoading();
        },
      );
      return () => {
        succListener.remove();
        failListener.remove();
      };
    }, [callDelayFuncQueue, dispatch]),
  );

  const goLogin = useCallback(async () => {
    // 当前手机不支持一键登录，跳转去普通登录页面
    if (!onePassEnable.current) {
      console.log('login');
      return;
    }
    // 等待设置一键登录UI页面完成
    await setUIConfig();
    // 拉起一键登录页面
    onePassSDK
      .onePass()
      .then(res => {})
      .catch(e => {
        console.log(e);
      });
  }, []);

  return useCallback(async () => {
    // SDK初始化成功后再调用去登录页面的函数
    if (onePassSDKIniting.current) {
      delayFuncQueue.current.push(goLogin);
      return;
    }
    goLogin();
  }, [goLogin]);
};

export default usePhoneLogin;
```

使用
```js
import React from 'react';
import {Button, StyleSheet, View} from 'react-native';

export const Login = () => {
  const onPhoneLogin = usePhoneLogin();

  return (
    <View style={styles.container}>
      <Button
        title="弹出登录"
        onPress={() => {
          onPhoneLogin();
        }}
      />
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
});
```