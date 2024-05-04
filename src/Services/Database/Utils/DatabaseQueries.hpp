/** @file DatabaseQueries.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
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
