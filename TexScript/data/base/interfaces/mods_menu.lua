local interface = {
	type = "interface",
	id = "INF_MODS",
	messages = {
		"MSG_MODS_MENU"
	},
	commands = {
		{ id = "CMD_BACK", flags = { "PopInf" } }
	}
}

data.extend({
	interface
})