#pragma once

namespace TexScript {

	class LocaleHandler
	{
	public:
		LocaleHandler() = default;
		~LocaleHandler() = default;

		void Load(const std::string& filepath);

		const std::string& Locale(const std::string& key);

		void Clear();

	private:
		void Insert(const std::string& key, const std::string& value);

	private:
		std::unordered_map<std::string, std::string> m_Locales;
	};

}