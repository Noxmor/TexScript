function onPushInterface(event)
	if(event.inf.id == "INF_TRAVEL") then
		print("HERE")
		for key, value in ipairs(data.current_loc.paths) do
			data.addCommand(event.inf.id, { id = "LOC_" .. value, loc_id = value, flags = { "PopInf", "Move" } })
		end
		data.addCommand(event.inf.id, { id = "CMD_BACK", flags = { "PopInf" } })
	end

	if(event.inf.id == "INF_LANGUAGE_OPTIONS") then
		for key, value in ipairs(data.languages) do
			data.addCommand(event.inf.id, { id = "CMD_" .. value, language_id = value, flags = { "PopInf", "Language" } })
		end
		data.addCommand(event.inf.id, { id = "CMD_BACK", flags = { "PopInf" } })
	end
end

function onPopInterface(event)
	if(event.inf.id == "INF_TRAVEL" or event.inf.id == "INF_LANGUAGE_OPTIONS") then
		data.clearCommands(event.inf.id)
	end
end