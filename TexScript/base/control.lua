function onPushInterface(event)
	if(event.inf.id == "INF_TRAVEL") then
		for key, value in ipairs(data.current_loc.paths) do
			local cmd = { id = "LOC_" .. value, loc_id = value, flags = { "PopInf", "Move" } }
			data.addCommand(event.inf.id, cmd)
		end
		data.addCommand(event.inf.id, { id = "CMD_BACK", flags = { "PopInf" } })
	end
end

function onPopInterface(event)
	if(event.inf.id == "INF_TRAVEL") then
		data.clearCommands(event.inf.id)
	end
end