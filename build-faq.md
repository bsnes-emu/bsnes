# Attempting to build the Cocoa frontend fails with NSInternalInconsistencyException

When building on macOS, the build system will make a native Cocoa app by default. In this case, the build system uses the Xcode `ibtool` command to build user interface files. If this command fails, you can fix this issue by starting Xcode and letting it install components. After this is done, you should be able to close Xcode and build successfully.

# Attempting to build the SDL frontend on macOS fails on linking

SameBoy on macOS expects you to have the SDL2 framework installed as a framework. You can find the SDL2 binaries on the [SDL homepage](https://www.libsdl.org/download-2.0.php). Mount the DMG and copy SDL2.framework to `/Library/Frameworks/`.
