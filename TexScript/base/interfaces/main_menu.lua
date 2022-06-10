local interface = {
	type = "interface",
	id = "INF_MAIN",
	main = true,
	messages = {
		"MSG_0",
		"MSG_1",
		"MSG_2"
	},
	commands = {
		{ id = "CMD_START", push_id = "INF_START", flags = { "PushInf" } },
		{ id = "CMD_MODS", push_id = "INF_MODS", flags = { "PushInf" } },
		{ id = "CMD_OPTIONS", push_id = "INF_OPTIONS", flags = { "PushInf" } },
		{ id = "CMD_QUIT", flags = { "Quit" } }
	}
}

data.extend({
	interface
})