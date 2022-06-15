#pragma once

#include "TexScript/Engine/Location/Location.hpp"

namespace TexScript {

	class LocationHandler
	{
	public:
		LocationHandler();
		~LocationHandler() = default;

		void RegisterLocation(const Location& loc);

		void Travel(const size_t id);

		const Location& GetLocation(const size_t id) const { return m_Locations.at(id); }
		Location& GetLocation(const size_t id) { return m_Locations.at(id); }

		const Location& CurrentLocation() const { return GetLocation(m_CurrentLocationID); }
		Location& CurrentLocation() { return GetLocation(m_CurrentLocationID); }

		size_t CurrentLocationID() const { return m_CurrentLocationID; }

		bool Validate() const;

	private:
		std::vector<Location> m_Locations;
		size_t m_CurrentLocationID = 0;
	};

}