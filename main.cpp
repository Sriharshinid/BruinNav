#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

void example(const MapLoader& ml)
{
	SegmentMapper sm;
	sm.init(ml); // let our object build its internal data structures
				 // by iterating thru all segments from the MapLoader object
	GeoCoord lookMeUp("34.0572000", "-118.4417620");
	std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp));
	if (vecOfAssociatedSegs.empty())
	{
		cout << "Error - no segments found matching this coordinate\n";
		return;
	}
	cout << "Here are all the segments associated with your coordinate:" << endl;
	for (auto s : vecOfAssociatedSegs)
	{
		cout << "Segment’s street: " << s.streetName << endl;
		cout << "Segment’s start lat/long: " << s.segment.start.latitude << ", " <<
			s.segment.start.longitude << endl;
		cout << "Segment’s end lat/long: " << s.segment.end.latitude << ", " <<
			s.segment.end.longitude << endl;
		cout << "This segment has " << s.attractions.size() <<
			" attractions on it." << endl;
	}
}




int main()
{
/*
	MapLoader ml;
	ml.load("mapdata.txt");
	GeoCoord gc;
	AttractionMapper cry;
	cry.init(ml);
	cry.getGeoCoord("Acosta Center", gc);
	SegmentMapper cry2;
	cry2.init(ml);

	vector<StreetSegment> ss;

	ss = cry2.getSegments(gc);

	for (int i = 0; i < ss.size(); i++)
	{
		cerr << ss[i].streetName << endl;
		cerr << ss[i].attractions.size() << endl;
	}
	

	//example(ml);
	*/
	
	Navigator nav;

	nav.loadMapData("mapdata.txt");
	vector<NavSegment> n;

	if (nav.navigate("cvs", "The Playboy Mansion", n) == NAV_SUCCESS)
		cout << "found" << endl;
	else
		cout << "NOT found" << endl;

	for (int a = 0; a < n.size(); a++)
	{
		if (n[a].m_command == 0)
		{
			cerr << "start: " << n[a].m_geoSegment.start.latitudeText << ", " << n[a].m_geoSegment.start.longitudeText << endl;
			cerr << "end: " << n[a].m_geoSegment.end.latitudeText << ", " << n[a].m_geoSegment.end.longitudeText << endl;
			cerr << "direction: " << n[a].m_direction << endl;
			cerr << "distance: " << n[a].m_distance << endl;
			cerr << "street: " << n[a].m_streetName << endl;
			cerr << endl;
		}
		else
		{
			cerr << "direction: " << n[a].m_direction << endl;
			cerr << "street: " << n[a].m_streetName << endl;
			cerr << endl;
		}

	}
		

}

