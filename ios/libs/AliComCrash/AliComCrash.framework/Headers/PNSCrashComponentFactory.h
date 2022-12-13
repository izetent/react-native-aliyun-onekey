//
//  PNSCrashComponentFactory.h
//  ATAuthSDK
//
//  Created by Vienta on 2020/2/6.
//  Copyright © 2020 alicom. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PNSCrashComponentInterface.h"

NS_ASSUME_NONNULL_BEGIN

@interface PNSCrashComponentFactory : NSObject

/**
 * 目前创建crash相关类
 * info 数据结构如下 {@"version":@"xxxx", @"filterModules":@[xxxx,xxxx], @"appid":@"xxxxx"}
 * version为版本号，filterModules为过滤条件
 * appid:应用的标识
 * 以上三个参数为必传项
 */
+ (id<PNSCrashComponentInterface>)createCrashComponent:(NSDictionary *)info;

@end

NS_ASSUME_NONNULL_END
