local interface = {
	type = "interface",
	id = "INF_OPTIONS",
	messages = {
		"MSG"
	},
	commands = {
		{ id = "CMD_BACK", flags = { "PopInf"} }
	}
}

data.extend({
	interface
})