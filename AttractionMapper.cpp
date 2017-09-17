#include "provided.h"
#include <string>
#include "MyMap.h"

using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> m_map;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
	m_map.clear();
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	StreetSegment strt;
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		ml.getSegment(i, strt);
		for (int j = 0; j < strt.attractions.size(); j++)
		{
			for (int c = 0; c < strt.attractions[j].name.size(); c++)
			{
				strt.attractions[j].name[c] = tolower(strt.attractions[j].name[c]);
			}
			m_map.associate(strt.attractions[j].name, strt.attractions[j].geocoordinates);
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	for (int c = 0; c < attraction.size(); c++)
	{
		attraction[c] = tolower(attraction[c]);
	}
	const GeoCoord *Gc = m_map.find(attraction);
	if (Gc != nullptr)
	{
		gc = *(Gc);
		return true;
	}
	else
		return false;  // This compiles, but may not be correct
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
