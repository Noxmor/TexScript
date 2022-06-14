local interface = {
	type = "interface",
	id = "INF_LOC",
	messages = {
		"MSG_LOC_INFO"
	},
	commands = {
		{ id = "CMD_TRAVEL", push_id = "INF_TRAVEL", flags = { "PushInf" } },
		{ id = "CMD_SEARCH", push_id = "INF_SEARCH", flags = { "PushInf" } },
		{ id = "CMD_INV", push_id = "INF_INV", flags = { "PushInf" } },
		{ id = "CMD_STATS", push_id = "INF_STATS", flags = { "PushInf" } },
		{ id = "CMD_OPTIONS", push_id = "INF_OPTIONS", flags = { "PushInf" } },
		{ id = "CMD_QUIT", flags = { "Quit" } }
	}
}

data.extend({
	interface
})