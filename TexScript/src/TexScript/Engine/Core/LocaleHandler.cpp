#include "tspch.hpp"

#include "LocaleHandler.hpp"

#include <fstream>

namespace TexScript {

	void LocaleHandler::Load(const std::string& filepath)
	{
		std::ifstream file(filepath);

		if (file.fail())
		{
			TS_ERROR("[LocaleHandler]: Failed to open file '{0}'!", filepath);
			file.close();
			return;
		}

		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty())
				continue;
			
			if (line.find('=') == std::string::npos)
			{
				TS_WARN("[LocaleHandler]: Error while reading line ({0}): Key and value are not seperated by '='!", filepath);
				continue;
			}

			if (line.at(0) == '=')
			{
				TS_WARN("[LocaleHandler]: Error while reading line ({0}): Line cannot start with '='!", filepath);
				continue;
			}

			const std::string value = line.substr(line.find('=') + 1);
			if (value.empty())
			{
				TS_WARN("[LocaleHandler]: Error while reading line ({0}): Value was not specified!", filepath);
				continue;
			}

			const std::string key = line.substr(0, line.find('='));
			
			Insert(key, value);
		}

		file.close();
	}

	const std::string& LocaleHandler::Locale(const std::string& key)
	{
		return m_Locales.find(key) != m_Locales.end() ? m_Locales.at(key) : key;
	}

	void LocaleHandler::Clear()
	{
		m_Locales.clear();
	}

	void LocaleHandler::Insert(const std::string& key, const std::string& value)
	{
		if (m_Locales.find(key) != m_Locales.end())
			TS_WARN("[LocaleHandler]: Overwriting locale with key: {0}", key);

		m_Locales[key] = value;
	}
}