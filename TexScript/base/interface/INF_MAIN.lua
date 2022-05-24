function onPushInterface(inf)
	ts_message(inf, "Welcome in the main menu")

	ts_command(inf, "CMD_START", PushInf, "INF_START")
	ts_command(inf, "CMD_MODS", PushInf, "INF_MODS")
	ts_command(inf, "CMD_OPTIONS", PushInf, "INF_OPTIONS")
	ts_command(inf, "CMD_QUIT", Quit)
end

function onPopInterface(inf)

end

function onRegisterInterface(inf)
	
end