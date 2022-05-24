#pragma once

namespace TexScript {

	enum CommandActionFlag
	{
		None = 0,

		Quit = BIT(0),
		PushInf = BIT(1),
		PopInf = BIT(2)
	};

}