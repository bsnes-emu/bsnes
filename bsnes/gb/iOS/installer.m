#import <Foundation/Foundation.h>
#import <dlfcn.h>
#import <objc/runtime.h>

@interface LSApplicationProxy : NSObject
@property (readonly, getter=isContainerized) bool containerized;
@property (readonly) NSString *bundleIdentifier;
@property (readonly) NSURL * bundleURL;
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
@property(readonly) NSURL *url;
@end


int main(int argc, char **argv)
{
    if (argc != 2) return 1;
    // Make sure MobileContainerManager is loaded
    if (!dlopen("/System/Library/PrivateFrameworks/MobileContainerManager.framework/MobileContainerManager", RTLD_NOW)) return 1;
    
    bool uninstall = false;
    if (strcmp(argv[1], "uninstall") == 0) {
        uninstall = true;
    }
    else if (strcmp(argv[1], "install") != 0) {
        return 1;
    }
    
    NSString *installPath = @"/var/jb/Applications/SameBoy-iOS.app";
    if (access("/Applications/", W_OK) == 0) {
        installPath = @"/Applications/SameBoy-iOS.app";
    }
    NSLog(@"Install path is %@", installPath);
    
    for (LSApplicationProxy *app in [[LSApplicationWorkspace defaultWorkspace] allInstalledApplications]) {
        if (![app.bundleIdentifier isEqualToString:[NSBundle mainBundle].bundleIdentifier]) continue;
        if (![app.bundleURL.path.stringByResolvingSymlinksInPath isEqual:installPath.stringByResolvingSymlinksInPath]) {
            // Already installed elsewhere
            NSLog(@"Already installed at %@", app.bundleURL.path);
            return 1;
        }
        
        NSLog(@"Unregistering previous installation");
        // We're registered but not containerized (or just uninstalling), unregister ourselves first
        if (![[LSApplicationWorkspace defaultWorkspace] unregisterApplication:app.bundleURL]) return 1;
        
        break;
    }
    
    // Don't modify files if we're at the correct path already
    if (uninstall || ![[NSBundle mainBundle].bundlePath.stringByResolvingSymlinksInPath isEqual:installPath.stringByResolvingSymlinksInPath]) {
        // Remove any previous copy
        NSError *error = nil;
        if (!access(installPath.UTF8String, F_OK)) {
            NSLog(@"Removing previous installation");
            [[NSFileManager defaultManager] removeItemAtPath:installPath error:&error];
            if (error) {
                NSLog(@"Error: %@", error);
                return 1;
            }
        }
        
        // If we're uninstalling, we're done
        if (uninstall) return 0;
        
        NSLog(@"Installing...");

        [[NSFileManager defaultManager] moveItemAtPath:[NSBundle mainBundle].bundlePath toPath:installPath error:&error];
        if (error) {
            NSLog(@"Error: %@", error);
            return 1;
        }
    }
    
    
    NSLog(@"Registering...");

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
        @"Path": installPath,
        @"_LSBundlePlugins": @{},
        @"IsContainerized": @YES,
    }];
}
