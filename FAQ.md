# Attempt to build Mac binary fails with NSInternalInconsistencyException

When building on the darwin platform SameBoy will attempt to make a native executable and UI. In this case the environment expects a bunch of stuff to be set up by XCode. Fix this issue by starting XCode and letting it install components and set up the environment. After it's done building SameBoy should work (wehther or not XCode continues to run).

# Attempt to build SDL binaris on Mac fails on linking

Sameboy expects you to have installed the SDL2 framework you can find the binaries on the [SLD homepage](https://www.libsdl.org/download-2.0.php). Mount the DMG and copy SDL2.framework to `/Library/Frameworks/`.
