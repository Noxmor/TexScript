local interface = {
	type = "interface",
	id = "INF_OPTIONS",
	messages = {
		"MSG_OPTIONS_MENU"
	},
	commands = {
		{ id = "CMD_LANGUAGE_OPTIONS", push_id = "INF_LANGUAGE_OPTIONS", flags = { "PushInf" } },
		{ id = "CMD_LAYOUT_OPTIONS", push_id = "INF_LAYOUT_OPTIONS", flags = { "PushInf" } },
		{ id = "CMD_BACK", flags = { "PopInf"} }
	}
}

data.extend({
	interface
})