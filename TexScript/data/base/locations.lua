local locations = {
	{
		type = "location",
		id = 0,
		region = "REG_LEVEL_7",
		locked = false,
		visible = true,
		paths = { 1 },
		items = {
			{ id = "ITEM_MEDICAL_HERB", count = 1 },
			{ id = "ITEM_MAGICAL_HERB", count = 1 }
		}
	},
	{
		type = "location",
		id = 1,
		region = "REG_LEVEL_7",
		locked = false,
		visible = true,
		paths = { 0, 2, 3, 4, 5, 6 }
	},
	{
		type = "location",
		id = 2,
		region = "REG_LEVEL_7",
		locked = false,
		visible = true,
		paths = { 1 }
	},
	{
		type = "location",
		id = 3,
		region = "REG_LEVEL_7",
		locked = false,
		visible = true,
		paths = { 1 }
	},
	{
		type = "location",
		id = 4,
		region = "REG_LEVEL_7",
		locked = false,
		visible = true,
		paths = { 1 }
	},
	{
		type = "location",
		id = 5,
		region = "REG_LEVEL_7",
		locked = false,
		visible = true,
		paths = { 1 }
	},
	{
		type = "location",
		id = 6,
		region = "REG_LEVEL_7",
		locked = false,
		visible = true,
		paths = { 1 }
	}
}

data.extend(locations)