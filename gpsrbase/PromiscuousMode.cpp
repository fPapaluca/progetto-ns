/*
 * PromiscuousMode.cpp
 *
 *  Created on: 27 set 2021
 *      Author: Francesco
 */

#include "PromiscuousMode.h"

void PromiscuousMode::registerHost(GpsrGrayholeSecure* host){
    mappa_host[host->getSelfAddress().str()] = host;
}

