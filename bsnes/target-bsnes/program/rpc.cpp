#include <debugapi.h>

auto Program::startRpcListener() -> void {
	rpcServer.main([&](nall::HTTP::Request& rq) -> nall::HTTP::Response {
		OutputDebugStringA("GOT REQUEST\n");
		nall::HTTP::Response resp;

		resp.setResponseType(200);
		resp.setText("hooray!");
		resp.header.append("Content-Type", "text/plain");
		return resp;
	});

	OutputDebugStringA("SETTING UP SERVER\n");
	rpcServer.open();
}

auto Program::stopRpcListener() -> void {
	rpcServer.close();
}

auto Program::processRpcCommands() -> void {
	rpcServer.scan();
}