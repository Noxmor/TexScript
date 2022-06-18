local interface = {
	type = "interface",
	id = "INF_NEW_GAME",
	messages = {
		"MSG_NEW_GAME"
	},
	custom_command = {
		push_id = "INF_LOC",
		flags = { "PushInf", "NewGame" }
	}
}

data.extend({
	interface
})