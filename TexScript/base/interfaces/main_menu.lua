local interface = {
	type = "interface",
	id = "INF_MAIN",
	main = true,
	messages = {
		"MSG_MAIN_MENU"
	},
	commands = {
		{ id = "CMD_PLAY", push_id = "INF_PLAY", flags = { "PushInf" } },
		{ id = "CMD_MODS", push_id = "INF_MODS", flags = { "PushInf" } },
		{ id = "CMD_OPTIONS", push_id = "INF_OPTIONS", flags = { "PushInf" } },
		{ id = "CMD_QUIT", flags = { "Quit" } }
	}
}

data.extend({
	interface
})