//
//  PNSCrashComponentInterface.h
//  ATAuthSDK
//
//  Created by Vienta on 2020/2/6.
//  Copyright © 2020 alicom. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^PNSCrashCallback)(NSString *crashUUID, NSException *exception);


@protocol PNSCrashComponentInterface <NSObject>

- (void)setupSDKCrashComponentWithCallback:(PNSCrashCallback)callback;

@end

NS_ASSUME_NONNULL_END
