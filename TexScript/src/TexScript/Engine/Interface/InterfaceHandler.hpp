#pragma once

#include <stack>

#include "TexScript/Engine/Interface/Interface.hpp"

namespace TexScript {

	class InterfaceHandler
	{
	public:
		InterfaceHandler() = default;
		~InterfaceHandler() = default;

		void RegisterInterface(const std::string& id, const Interface& inf);

		const Interface& GetInterface(const std::string& id) const { return m_Interfaces.at(id); }
		Interface& GetInterface(const std::string& id) { return m_Interfaces.at(id); }

		const Interface& CurrentInterface() const;
		Interface& CurrentInterface();

		const std::string& CurrentInterfaceID() const;

		void Push(const std::string& id);
		void Pop();

		bool HasInterfaceID(const std::string& id) const { return m_Interfaces.find(id) != m_Interfaces.end(); }

	private:
		std::stack<std::string> m_InterfaceStack;
		std::unordered_map<std::string, Interface> m_Interfaces;
	};

}