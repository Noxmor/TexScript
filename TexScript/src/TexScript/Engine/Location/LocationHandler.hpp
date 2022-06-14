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

		const Location& CurrentLocation() const { return m_Locations.at(m_CurrentLocationID); }
		Location& CurrentLocation() { return m_Locations.at(m_CurrentLocationID); }

		size_t CurrentLocationID() const { return m_CurrentLocationID; }

		bool Validate() const;

	private:
		std::vector<Location> m_Locations;
		size_t m_CurrentLocationID = 0;
	};

}