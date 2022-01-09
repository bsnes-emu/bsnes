#include <debugapi.h>

auto Program::startRpcListener() -> void {
	rpcServer.main([&](nall::HTTP::Request& rq) -> nall::HTTP::Response {
		nall::HTTP::Response resp;

		if (rq.requestType() != nall::HTTP::Request::RequestType::Post) {
			resp.setResponseType(200);
			resp.header.append("Content-Type", "text/plain");
			resp.setText("no");

			return resp;
		}

		rq.body([&](const uint8_t *data, nall::uint size) -> bool {
			if (size > 16*1024) {
				resp.setResponseType(400);
				return true;
			}

			string content((char*)data);

			auto idx = content.find("|");
			if (!idx) {
				resp.setResponseType(400);
				resp.setText("Invalid Command!!\n");
				resp.header.append("Content-Type", "text/plain");
				return true;
			}

			auto cmd = content.slice(0, idx.get());
			auto arg = content.slice(idx.get() + 1);
			RpcCommand rpcCmd;
			if (cmd == "pau") {
				rpcCmd.type = RpcCommandType::Pause;
				pendingRpcCommands.write(rpcCmd);
			}

			resp.setResponseType(200);
			resp.setText("ok\n");
			resp.header.append("Content-Type", "text/plain");

			return true;
		});

		return resp;
	});

	rpcServer.open();
}

auto Program::stopRpcListener() -> void {
	rpcServer.close();
}

auto Program::processRpcCommands() -> void {
	rpcServer.scan();

	RpcCommand cmd;
	if (pendingRpcCommands.read(cmd)) {
		if (cmd.type == RpcCommandType::Pause) {
			if (presentation.runEmulation.checked()) {
				presentation.pauseEmulation.setChecked().doActivate();
			} else {
				// unpausing can also cancel frame advance mode
				presentation.runEmulation.setChecked().doActivate();
			}
		}
	}
}