#pragma once

#include <unordered_map>

namespace TexScript {

	class LuaTable
	{
	public:
		LuaTable() = default;
		~LuaTable() = default;

		void Print(const size_t level = 0) const
		{
			std::string levelLine;
			for (size_t i = 0; i < level; i++)
			{
				levelLine += "   ";
			}

			for (auto it = m_StringData.begin(); it != m_StringData.end(); ++it)
			{
				TS_TRACE("[LuaTable]: " + levelLine + "(Data)Key: {0}, Value : {1}", it->first, it->second);
			}

			for (auto it = m_IntData.begin(); it != m_IntData.end(); ++it)
			{
				TS_TRACE("[LuaTable]: " + levelLine + "(Data)Key: {0}, Value : {1}", it->first, it->second);
			}

			for (auto it = m_BoolData.begin(); it != m_BoolData.end(); ++it)
			{
				TS_TRACE("[LuaTable]: " + levelLine + "(Data)Key: {0}, Value : {1}", it->first, it->second);
			}

			for (auto it = m_TableData.begin(); it != m_TableData.end(); ++it)
			{
				TS_TRACE("[LuaTable]: " + levelLine + "(Table)Name: {0}", it->first);
				it->second.Print(level + 1);
			}
		}

		void Insert(const std::string& key, const std::string& value) { m_StringData[key] = value; }
		void Insert(const std::string& key, const int32_t value) { m_IntData[key] = value; }
		void Insert(const std::string& key, const bool value) { m_BoolData[key] = value; }
		void Insert(const std::string& key, const LuaTable& table) { m_TableData[key] = table; }

		bool HasStringData(const std::string& key) const { return m_StringData.find(key) != m_StringData.end(); }
		bool HasIntData(const std::string& key) const { return m_IntData.find(key) != m_IntData.end(); }
		bool HasBoolData(const std::string& key) const { return m_BoolData.find(key) != m_BoolData.end(); }
		bool HasTableData(const std::string& key) const { return m_TableData.find(key) != m_TableData.end(); }

		const std::string& StringData(const std::string& key) const { return m_StringData.at(key); }
		std::string& StringData(const std::string& key) { return m_StringData.at(key); }
		const std::unordered_map<std::string, std::string>& StringData() const { return m_StringData; }

		const int32_t& IntData(const std::string& key) const { return m_IntData.at(key); }
		int32_t IntData(const std::string& key) { return m_IntData.at(key); }
		const std::unordered_map<std::string, int32_t>& IntData() const { return m_IntData; }

		const bool& BoolData(const std::string& key) const { return m_BoolData.at(key); }
		bool BoolData(const std::string& key) { return m_BoolData.at(key); }
		const std::unordered_map<std::string, bool>& BoolData() const { return m_BoolData; }

		const LuaTable& TableData(const std::string& key) const { return m_TableData.at(key); }
		LuaTable& TableData(const std::string& key) { return m_TableData.at(key); }
		const std::unordered_map<std::string, LuaTable>& TableData() const { return m_TableData; }

	private:
		std::unordered_map<std::string, std::string> m_StringData;
		std::unordered_map<std::string, int32_t> m_IntData;
		std::unordered_map<std::string, bool> m_BoolData;
		std::unordered_map<std::string, LuaTable> m_TableData;
	};

}