#include "geometry_list.h"

using namespace raytracer;

GeometryList &GeometryList::operator=(const GeometryList &geoListObj)
{
    geoList = geoListObj.geoList;
    return *this;
}

void GeometryList::add(shared_ptr<Geometry> geo)
{
    geoList.push_back(geo);
}

void GeometryList::clear()
{
    geoList.clear();
}

bool GeometryList::isHit(const Ray &ray, double tMin, double tMax, HitInfo &hitInfo) const
{
    HitInfo tempHitInfo;
    double closestHitDist = tMax;
    bool isHit = false;

    for (const shared_ptr<Geometry> &g : geoList)
    {
        if (g->isHit(ray, tMin, closestHitDist, tempHitInfo))
        {
            hitInfo = tempHitInfo;
            closestHitDist = hitInfo.distInRay;
            isHit = true;
        }
    }

    return isHit;
}