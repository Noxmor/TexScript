local interface = {
	type = "interface",
	id = "INF_PLAY",
	messages = {
		"MSG_PLAY_MENU"
	},
	commands = {
		{ id = "CMD_NEW_GAME", push_id = "INF_NEW_GAME", flags = { "PushInf" } },
		{ id = "CMD_LOAD_GAME", push_id = "INF_LOAD_GAME", flags = { "PushInf" } },
		{ id = "CMD_BACK", flags = { "PopInf" } }
	}
}

data.extend({
	interface
})