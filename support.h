#ifndef SUPPORT_H
#define SUPPORT_H

#include "provided.h"

bool operator< (const GeoCoord& lhs, const GeoCoord& rhs);
bool operator==(const GeoCoord& lhs, const GeoCoord& rhs);
bool operator>(const GeoCoord& lhs, const GeoCoord& rhs);

#endif // !SUPPORT_H



