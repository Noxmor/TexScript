function onPushInterface(event)
	if(event.inf.id == "INF_TRAVEL") then
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

	if(event.inf.id == "INF_MODS") then
		local i, t, popen = 0, {}, io.popen
		local pfile = popen('dir "'.."data"..'" /b /ad')
		for dirName in pfile:lines() do
			data.addCommand(event.inf.id, { id = dirName, flags = { "PopInf", "Mod" } })
		end
		pfile:close()
		data.addCommand(event.inf.id, { id = "CMD_BACK", flags = { "PopInf" } })
	end

	if(event.inf.id == "INF_INV") then
		for key, value in ipairs(data.inventory.items) do
			data.addCommand(event.inf.id, { id = value.id .. " (x" .. value.count .. ")", flags = { "PopInf" } })
		end
		data.addCommand(event.inf.id, { id = "CMD_BACK", flags = { "PopInf"} })
	end
end

function onPopInterface(event)
	if(event.inf.id == "INF_TRAVEL" or event.inf.id == "INF_LANGUAGE_OPTIONS" or event.inf.id == "INF_MODS" or event.inf.id == "INF_INV") then
		data.clearCommands(event.inf.id)
	end
end