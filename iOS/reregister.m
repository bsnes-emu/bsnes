#import <Foundation/Foundation.h>
#import <dlfcn.h>
#import <objc/runtime.h>

@interface LSApplicationProxy : NSObject
@property (nonatomic, readonly, getter=isContainerized) bool containerized;
@property (nonatomic, readonly) NSString *bundleIdentifier;
@end

@interface LSApplicationWorkspace : NSObject
+ (instancetype)defaultWorkspace;
- (NSArray <LSApplicationProxy *> *)allInstalledApplications;
- (bool)unregisterApplication:(NSURL *)url;
- (bool)registerApplicationDictionary:(NSDictionary *)dict;
@end

@interface MCMAppDataContainer : NSObject
+ (MCMAppDataContainer *)containerWithIdentifier:(NSString *)identifier
                               createIfNecessary:(bool)create
                                         existed:(bool *)existed
                                           error:(NSError **)error;
@property(readonly, nonatomic) NSURL *url;
@end


int main(void)
{
    // Make sure MobileContainerManager is loaded
    if (!dlopen("/System/Library/PrivateFrameworks/MobileContainerManager.framework/MobileContainerManager", RTLD_NOW)) return 1;
    for (LSApplicationProxy *app in [[LSApplicationWorkspace defaultWorkspace] allInstalledApplications]) {
        if (![app.bundleIdentifier isEqualToString:[NSBundle mainBundle].bundleIdentifier]) continue;
        if (app.containerized) return 0; // Everything's fine, no need to reregister
        // We're registered but not containerized, unregister ourselves first
        if (![[LSApplicationWorkspace defaultWorkspace] unregisterApplication:[NSBundle mainBundle].bundleURL]) return 1;
        
        break;
    }
    
    NSString *container = [objc_getClass("MCMAppDataContainer") containerWithIdentifier:[NSBundle mainBundle].bundleIdentifier
                                                                      createIfNecessary:true
                                                                                existed:nil
                                                                                  error:nil].url.path;
    
    return ![[LSApplicationWorkspace defaultWorkspace] registerApplicationDictionary:@{
        @"ApplicationType": @"System",
        @"CFBundleIdentifier": [NSBundle mainBundle].bundleIdentifier,
        @"CompatibilityState": @NO,
        @"Container": container,
        @"IsDeletable": @NO,
        @"Path": [NSBundle mainBundle].bundlePath,
        @"_LSBundlePlugins": @{},
        @"IsContainerized": @YES,
    }];
}
