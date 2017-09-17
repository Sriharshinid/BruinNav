#include "provided.h"
#include "support.h"

bool operator< (const GeoCoord& lhs, const GeoCoord& rhs)
{
	if (lhs.latitudeText < rhs.latitudeText)
		return true;
	else if (lhs.latitudeText > rhs.latitudeText)
		return false;
	else
		return (lhs.longitudeText < rhs.longitudeText);
}

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs)
{
	return (lhs.latitudeText == rhs.latitudeText && lhs.longitudeText == rhs.longitudeText);
}

bool operator>(const GeoCoord& lhs, const GeoCoord& rhs)
{
	return !(lhs < rhs);
}