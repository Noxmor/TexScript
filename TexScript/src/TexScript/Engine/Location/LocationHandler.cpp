#include "tspch.hpp"

#include "LocationHandler.hpp"

namespace TexScript {

	LocationHandler::LocationHandler()
	{
		//Ensures there will always exist at least one location that CurrentLocation() can return.
		//Usually, the user will overwrite this default location.

		Location defaultLoc;
		RegisterLocation(defaultLoc);
	}

	void LocationHandler::RegisterLocation(const Location& loc)
	{
		if (loc.LocationID >= m_Locations.size())
			m_Locations.resize(loc.LocationID + 1);

		m_Locations[loc.LocationID] = loc;
	}

	void LocationHandler::Travel(const size_t id)
	{
		if (m_CurrentLocationID == id)
		{
			TS_WARN("[LocationHandler]: Cannot travel to current location!");
			return;
		}
		
		if (id > m_Locations.size() - 1)
		{
			TS_ERROR("[LocationHandler]: Location with id {0} does not exist!", id);
			return;
		}

		if(!m_Locations.at(id).Locked)
			m_CurrentLocationID = id;
	}

	bool LocationHandler::Validate() const
	{
		for (size_t i = 0; i < m_Locations.size(); i++)
		{
			if (m_Locations.at(i).LocationID != i)
				return false;
		}

		return true;
	}

}