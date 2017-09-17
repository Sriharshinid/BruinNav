#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <iostream>
#include <vector>
using namespace std;



class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<GeoCoord, vector<StreetSegment>> m_Map;
	void addSegtoMap(StreetSegment ss, GeoCoord geo);
	
};



SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
	
	m_Map.clear();
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	StreetSegment ss;
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		ml.getSegment(i, ss);


		addSegtoMap(ss, ss.segment.start);
		addSegtoMap(ss, ss.segment.end);
		
		for (int j = 0; j < ss.attractions.size(); j++)
		{
			if (ss.attractions[j].geocoordinates == ss.segment.start || ss.attractions[j].geocoordinates == ss.segment.end)
				continue;
			addSegtoMap(ss, ss.attractions[j].geocoordinates);
		}
	}
}

void SegmentMapperImpl::addSegtoMap(StreetSegment s, GeoCoord geo)
{
	vector<StreetSegment> *p = m_Map.find(geo);
	if (p != nullptr)
		p->push_back(s);
	else
	{
		vector<StreetSegment> temp;
		temp.push_back(s);
		m_Map.associate(geo, temp);
	}

}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	const vector<StreetSegment> segments;
	const vector<StreetSegment> *seg = m_Map.find(gc);
	const vector<StreetSegment> segg = *seg;

	if (seg == nullptr)
		return segments;
	else 
	{
		return segg;
	}// This compiles, but may not be correct
}




//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
