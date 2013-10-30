/*
 * ConservedMoietyPlugin.cpp
 *
 *  Created on: Oct 18, 2013
 *      Author: andy
 */

#include "conservation/ConservedMoietyPlugin.h"


namespace rr { namespace conservation {


rr::conservation::ConservedMoietyPlugin::ConservedMoietyPlugin(
        const std::string& uri, const std::string& prefix,
        ConservationPkgNamespaces* consns)
: libsbml::SBasePlugin(uri, prefix, consns)
{
}

rr::conservation::ConservedMoietyPlugin::ConservedMoietyPlugin(
        const ConservedMoietyPlugin& orig)
: libsbml::SBasePlugin(orig)
{
}

rr::conservation::ConservedMoietyPlugin::~ConservedMoietyPlugin()
{
}

ConservedMoietyPlugin& rr::conservation::ConservedMoietyPlugin::operator =(
        const ConservedMoietyPlugin& orig)
{
    return *this;
}

ConservedMoietyPlugin* rr::conservation::ConservedMoietyPlugin::clone() const
{
    return 0;
}

bool ConservedMoietyPlugin::getConservedMoiety()
{
    return false;
}

void ConservedMoietyPlugin::setConservedMoiety(bool value)
{
}

}
} // namespace rr } namespace conservation }

