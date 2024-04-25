/** @file DatabaseQueries.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  RBC         | 2.3.0
 *
 *  ### Description
 *  List of sql statements to be used
 *
 *  ### Contributors
 *  veselo21
 */

#pragma once

#include <string>

/**
 * Column 1 baliseID
 * Column 2 telegram
 * Column 3 deltaPosition
 */
const std::string loadBaliseInfo =
    "select balise.id, balise.defaultTelegram, ap.deltaPosition from balise \n"
    "join associatedlocationentity ale on balise.id=ale.id_NetEntity\n"
    "join associatedlocationfeature alf on ale.id_AssociatedLocation=alf.id_AssociatedLocation\n"
    "join associatedposition ap on alf.id_AssociatedFeature=ap.id\n"
    "order by ap.deltaPosition";

const std::string baliseGroupInfo = "select balise.defaultTelegram from balise";

const std::string speedSectionPointA =
    "select ss.id, ss.defaultMaximalSpeed, ap.deltaPosition from speedsection ss\n"
    "join associatedlocationentity ale on ss.id=ale.id_NetEntity\t\n"
    "join associatedlocationfeature alf on ale.id_AssociatedLocation=alf.id_AssociatedLocation\n"
    "join associatedsection ass on alf.id_AssociatedFeature=ass.id\n"
    "join associatedposition ap on ass.id_AssociatedPosition_A=ap.id\n"
    "order by ap.deltaPosition";
const std::string speedSectionPointB =
    "select ss.id, ss.defaultMaximalSpeed, ap.deltaPosition from speedsection ss\n"
    "join associatedlocationentity ale on ss.id=ale.id_NetEntity\t\n"
    "join associatedlocationfeature alf on ale.id_AssociatedLocation=alf.id_AssociatedLocation\n"
    "join associatedsection ass on alf.id_AssociatedFeature=ass.id\n"
    "join associatedposition ap on ass.id_AssociatedPosition_B=ap.id\n"
    "order by ap.deltaPosition";

const std::string verticalCurvePointA =
    "select vc.id, vc.horizontalLength, vc.elevation0, vc.deltaElevation, ap.deltaPosition from verticalcurve vc\n"
    "join associatedlocationentity ale on vc.id=ale.id_NetEntity\n"
    "join associatedlocationfeature alf on ale.id_AssociatedLocation=alf.id_AssociatedLocation\n"
    "join associatedsection ass on alf.id_AssociatedFeature=ass.id\n"
    "join associatedposition ap on ass.id_AssociatedPosition_A=ap.id\n"
    "order by ap.deltaPosition;";

const std::string verticalCurvePointB =
    "select vc.id, vc.horizontalLength, vc.elevation0, vc.deltaElevation, ap.deltaPosition from verticalcurve vc\n"
    "join associatedlocationentity ale on vc.id=ale.id_NetEntity\n"
    "join associatedlocationfeature alf on ale.id_AssociatedLocation=alf.id_AssociatedLocation\n"
    "join associatedsection ass on alf.id_AssociatedFeature=ass.id\n"
    "join associatedposition ap on ass.id_AssociatedPosition_B=ap.id\n"
    "order by ap.deltaPosition;";
