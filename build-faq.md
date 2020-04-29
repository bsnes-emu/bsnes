# Attempting to build the Cocoa frontend fails with NSInternalInconsistencyException

When building on macOS, the build system will make a native Cocoa app by default. In this case, the build system uses the Xcode `ibtool` command to build user interface files. If this command fails, you can fix this issue by starting Xcode and letting it install components. After this is done, you should be able to close Xcode and build successfully.

# Attempting to build the SDL frontend on macOS fails on linking

SameBoy on macOS expects you to have SDL2 installed via Brew, and not as a framework. Older versions expected it to be installed as a framework, but this is no longer the case.