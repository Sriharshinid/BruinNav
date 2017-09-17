#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <iostream>
using namespace std;


class NavigatorImpl
{
public:
	NavigatorImpl();
	~NavigatorImpl();
	bool loadMapData(string mapFile);
	NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	struct point
	{
		point(string name, GeoCoord c, double distS = 0, double distTot = 0, point *prev = nullptr)
		{
			m_name = name;
			m_distFromStart = distS;
			m_totalDist = distTot;
			m_coordPoint = c;
			m_prev = prev;
		}

		point()
		{
			m_name = "";
			m_distFromStart = 0;
			m_totalDist = 0;
			m_prev = nullptr;
			GeoCoord g;
			m_coordPoint = g;
		}
		string m_name;
		double m_distFromStart;
		double m_totalDist;

		GeoCoord m_coordPoint;
		point *m_prev;
		point& operator=(const point& other)
		{
			this->m_name = other.m_name;
			this->m_coordPoint = other.m_coordPoint;
			this->m_distFromStart = other.m_distFromStart;
			this->m_prev = other.m_prev;
			this->m_totalDist = other.m_totalDist;
			return *this;
		}
	};

	AttractionMapper atMap;
	SegmentMapper segMap;

	void createNewPoint(point& np, point* prevPoint, StreetSegment* Seg, GeoCoord* endCoord, GeoCoord newc) const;
	void createVectorofDirections(point* endd, vector<NavSegment> &dir) const;
	vector<GeoCoord> getVerticies(StreetSegment *stseg, GeoCoord ge) const;
	string leftORright(double deg) const;
	string NESW(double deg) const;
	static bool comparison(point* a, point* b);
};

bool NavigatorImpl::comparison(point* a, point* b)
{

	if (a->m_totalDist > b->m_totalDist)
		return true;
	else
		return false;
}

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
	//?????????????
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	MapLoader ml;
	if (!ml.load(mapFile))
		return false;

	atMap.init(ml);
	segMap.init(ml);
	return true;  // This compiles, but may not be correct
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{

	for (int c = 0; c < start.size(); c++)
	{
		start[c] = tolower(start[c]);
	}
	for (int c = 0; c < end.size(); c++)
	{
		end[c] = tolower(end[c]);
	}

	priority_queue< point*, vector<point*>, decltype(&NavigatorImpl::comparison)> open(&NavigatorImpl::comparison);


	GeoCoord startGeo;
	GeoCoord endGeo;
	MyMap<GeoCoord, point*> closed;
	vector<point*> closedCopy;

	//CONVERT START AND END TO ALL LOWER CASE LETTERS!!!!!

	if (!atMap.getGeoCoord(start, startGeo))
		return NAV_BAD_SOURCE;
	if (!atMap.getGeoCoord(end, endGeo))
		return NAV_BAD_DESTINATION;

	//initializing the starting point and loading it with info
	point* startPoint = new point(start, startGeo, 0, distanceEarthMiles(startGeo, endGeo), nullptr);
	open.push(startPoint);

	//initializing the end point and loading it with info
	point* endPoint = new point(end, endGeo, 0, 0, nullptr);


	while (!open.empty())
	{
		point* prevPt = open.top();
		open.pop();
		if (closed.find(prevPt->m_coordPoint) != nullptr)
		{
			delete prevPt;
			continue;
		}
		closed.associate(prevPt->m_coordPoint, prevPt);
		closedCopy.push_back(prevPt);
		vector<StreetSegment> segs = segMap.getSegments(prevPt->m_coordPoint);

		for (int x = 0; x < segs.size(); x++)
		{
			for (int y = 0; y < segs[x].attractions.size(); y++)
			{
				if (segs[x].attractions[y].geocoordinates == endGeo)
				{
					endPoint->m_prev = prevPt;
					endPoint->m_distFromStart = endPoint->m_prev->m_distFromStart + distanceEarthMiles(endGeo, (prevPt->m_coordPoint));
					endPoint->m_totalDist = endPoint->m_distFromStart;
					closed.associate(endPoint->m_coordPoint, endPoint);
					closedCopy.push_back(endPoint);
					directions.clear();
					createVectorofDirections(endPoint, directions);
					cerr << endPoint->m_distFromStart << endl;
					while (!open.empty())
					{
						point *blub = open.top();
						open.pop();
						delete blub;
					}
					while (!closedCopy.empty())
					{
						delete closedCopy[closedCopy.size() - 1];
						closedCopy.pop_back();
					}
					closed.clear();
					//DELETE POINTS FROM PRIORITY QUEUE
					//DELETE POINTS FROM MYMAP
					return NAV_SUCCESS;
				}
			}

			//add points that i need to add here

			//get the correct vertex
			vector<GeoCoord> vect = getVerticies(&segs[x], prevPt->m_coordPoint);
			for (int v = 0; v < vect.size(); v++)
			{
				point *newpt = new point(segs[x].streetName, vect[v], prevPt->m_distFromStart + distanceEarthMiles(vect[v], (prevPt->m_coordPoint)), prevPt->m_distFromStart + distanceEarthMiles(vect[v], (prevPt->m_coordPoint)) + distanceEarthMiles((vect[v]), endGeo), prevPt);
				//createNewPoint(newpt, &prevPt, &segs[x], &endGeo, vect[v]);
				if (closed.find(newpt->m_coordPoint) == nullptr)
					open.push(newpt);
				else
					delete newpt;
			}
		}

	}

	while (!open.empty())
	{
		point *blub = open.top();
		open.pop();
		delete blub;
	}
	while (!closedCopy.empty())
	{
		delete closedCopy[closedCopy.size() - 1];
		closedCopy.pop_back();
	}
	closed.clear();


	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}


vector<GeoCoord> NavigatorImpl::getVerticies(StreetSegment *stseg, GeoCoord ge) const
{
	vector<GeoCoord> returnVal;
	if ((stseg->segment).start == ge)
		returnVal.push_back((stseg->segment).end);
	else if ((stseg->segment).end == ge)
	{
		returnVal.push_back((stseg->segment).start);
	}
	else
	{
		returnVal.push_back((stseg->segment).end);
		returnVal.push_back((stseg->segment).start);
	}
	return returnVal;
}


void NavigatorImpl::createNewPoint(point& np, point* prevPoint, StreetSegment* Seg, GeoCoord* endCoord, GeoCoord newc) const
{
	np.m_name = Seg->streetName;
	//cerr << np.m_name << endl;
	np.m_prev = prevPoint;
	np.m_coordPoint = newc;
	np.m_distFromStart = np.m_prev->m_distFromStart + distanceEarthMiles((np.m_coordPoint), ((np.m_prev)->m_coordPoint));
	np.m_totalDist = np.m_distFromStart + distanceEarthMiles((np.m_coordPoint), *endCoord);
}

void NavigatorImpl::createVectorofDirections(point *endd, vector<NavSegment> &dir) const
{
	string endName = endd->m_name;
	vector<NavSegment> reverse;
	GeoSegment geoseg;
	GeoSegment geosegExtra;

	while (endd != nullptr)
	{
		if (endd->m_prev == nullptr)
			break;
		if (endd->m_prev != nullptr)
		{
			geoseg.start = (endd->m_prev)->m_coordPoint;
			geoseg.end = endd->m_coordPoint;
			string namee = endd->m_name;
			if (namee == endName)
				namee = endd->m_prev->m_name;
			NavSegment nP(NESW(angleOfLine(geoseg)), namee, distanceEarthMiles(geoseg.start, geoseg.end), geoseg);
			reverse.push_back(nP);
		}
		if (endd->m_prev->m_prev != nullptr && endd->m_name != endd->m_prev->m_name && endName != endd->m_name)
		{
			geoseg.start = endd->m_prev->m_prev->m_coordPoint;
			geoseg.end = endd->m_prev->m_coordPoint;
			geosegExtra.end = endd->m_coordPoint;
			geosegExtra.start = endd->m_prev->m_coordPoint;
			NavSegment nT(leftORright(angleBetween2Lines(geoseg, geosegExtra)), endd->m_name);
			reverse.push_back(nT);
		}

		endd = endd->m_prev;
	}


	//makes the dirs vector the right order
	while (!reverse.empty())
	{
		dir.push_back(reverse.back());
		reverse.pop_back();
	}

}

string NavigatorImpl::leftORright(double deg) const
{
	if (deg < 180)
		return "left";
	else
		return "right";
}
string NavigatorImpl::NESW(double deg) const
{
	if (deg <= 22.5)
		return "east";
	else if (deg <= 67.5)
		return "northeast";
	else if (deg <= 112.5)
		return "north";
	else if (deg <= 157.5)
		return "northwest";
	else if (deg <= 202.5)
		return "west";
	else if (deg <= 247.5)
		return "southwest";
	else if (deg <= 292.5)
		return "south";
	else if (deg <= 337.5)
		return "southeast";
	else
		return "east";
}


//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
	m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
	delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
	return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
	return m_impl->navigate(start, end, directions);
}

