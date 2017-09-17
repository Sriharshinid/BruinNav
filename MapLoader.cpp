#include "provided.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cctype>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> m_streets;
};


MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);    // infile is a name of our choosing
	if (!infile)		        // Did opening the file fail?
	{
		cerr << "Error: Cannot open mapdata.txt!" << endl;
		return false;
	}
	std::string s;
	while (getline(infile, s))
	{
		StreetSegment ss;

		//(m_streets[m_streets.size() - 1]).streetName = s;
		ss.streetName = s;

		string stCoordX;
		getline(infile, stCoordX, ',');
		string stCoordY;
		//getline(infile, stCoordY, ' ');	//deletes the space
		char p;
		infile.get(p);
		getline(infile, stCoordY, ' ');
		if (p != ' ')
			stCoordY = p + stCoordY;
		string enCoordX;
		getline(infile, enCoordX, ',');
		string enCoordY;
		infile.get(p);
		getline(infile, enCoordY);
		if (p != ' ')
			enCoordY = p + enCoordY;

		//cerr << stCoordX << '\t' << stCoordY << '\t' << enCoordX << '\t' << enCoordY << endl;

		GeoCoord start(stCoordX, stCoordY);
		GeoCoord end(enCoordX, enCoordY);
		GeoSegment seg(start, end);
		//(m_streets[m_streets.size() - 1]).segment= seg;
		ss.segment = seg;
		int numAttraction;
		infile >> numAttraction;
		infile.ignore(10000, '\n');

		//cerr << numAttraction << endl;

		for (int i = 0; i < numAttraction; i++)
		{
			Attraction at;
			string atName;
			getline(infile, atName, '|');
			at.name = atName;
			//cerr << atName << endl;
			//cerr << "name: " << atName << endl;
			string sCx;
			getline(infile, sCx, ',');
			string sCy;
			infile.get(p);
			getline(infile, sCy);
			if (p != ' ')
				sCy = p + sCy;
			GeoCoord gC(sCx, sCy);
			at.geocoordinates = gC;
			//(m_streets[m_streets.size() - 1]).attractions.push_back(at);
			ss.attractions.push_back(at);
		}
		m_streets.push_back(ss);
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_streets.size(); // This compiles, but may not be correct
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < 0 || segNum >= getNumSegments())
		return false;
	seg = m_streets[segNum];
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
