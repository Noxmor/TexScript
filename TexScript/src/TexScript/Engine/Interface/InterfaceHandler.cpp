#include "tspch.hpp"

#include "InterfaceHandler.hpp"

namespace TexScript {

	void InterfaceHandler::RegisterInterface(const std::string& id, const Interface& inf)
	{
		if (m_Interfaces.find(id) != m_Interfaces.end())
			TS_WARN("[InterfaceHandler]: Overwriting already existing interface with id: {0}", id);

		m_Interfaces[id] = inf;
	}

	const Interface& InterfaceHandler::CurrentInterface() const
	{
		TS_ASSERT(m_Interfaces.size(), "[InterfaceHandler]: No interfaces exists!");
		TS_ASSERT(m_InterfaceStack.size(), "[InterfaceHandler]: There exists no current interface!");
		TS_ASSERT(m_Interfaces.find(m_InterfaceStack.top()) != m_Interfaces.end(), "[InterfaceHandler]: Invalid Interface ID!");

		return m_Interfaces.at(m_InterfaceStack.top());
	}

	Interface& InterfaceHandler::CurrentInterface()
	{
		TS_ASSERT(m_Interfaces.size(), "[InterfaceHandler]: No interfaces exists!");
		TS_ASSERT(m_InterfaceStack.size(), "[InterfaceHandler]: There exists no current interface!");
		TS_ASSERT(m_Interfaces.find(m_InterfaceStack.top()) != m_Interfaces.end(), "[InterfaceHandler]: Invalid Interface ID!");

		return m_Interfaces.at(m_InterfaceStack.top());
	}

	const std::string& InterfaceHandler::CurrentInterfaceID() const
	{
		TS_ASSERT(m_InterfaceStack.size(), "[InterfaceHandler]: There exists no current interface!");
		return m_InterfaceStack.top();
	}

	void InterfaceHandler::Push(const std::string& id)
	{
		m_InterfaceStack.push(id);
	}

	void InterfaceHandler::Pop()
	{
		if (m_InterfaceStack.size() < 2)
		{
			TS_WARN("[InterfaceHandler]: Cannot pop last remaining interface!");
			return;
		}

		m_InterfaceStack.pop();
	}

}