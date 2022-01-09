#include <debugapi.h>

auto Program::startRpcListener() -> void {
	// NB: There is no cross-platform way to read from a pipe in a non-blocking 
	// way. So instead, we're going to spin up a blocking thread that reads stdin
	// and processes it. What's even worse, is that we can't even properly 
	// terminate this thread since there's no timeout on blocking stdin reads,
	// all we can do is force-terminate this thread on exit.
	rpcHandlerThread = nall::thread::create([&](auto param) {
		OutputDebugStringA("HELLO IT WORKED LOOK AT THIS\n");
	});
}

auto Program::stopRpcListener() -> void {
	rpcHandlerThread.cancel();
}

auto Program::processRpcCommands() -> void {
}