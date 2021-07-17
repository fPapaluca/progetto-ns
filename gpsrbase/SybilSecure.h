/*
 * SybilSecure.h
 *
 *  Created on: 17 lug 2021
 *      Author: Francesco
 */

#ifndef GPSRBASE_SYBILSECURE_H_
#define GPSRBASE_SYBILSECURE_H_

#include "GpsrSecure.h"
#include <iostream>
#include <string>
#include "../gpsrbase/GpsrBase_m.h"
#include "../gpsrbase/GpsrPositionTable.h"
#include "inet/common/INETDefs.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/common/packet/Packet.h"
#include "inet/mobility/contract/IMobility.h"
#include "inet/networklayer/contract/IL3AddressType.h"
#include "inet/networklayer/contract/INetfilter.h"
#include "inet/networklayer/contract/IRoutingTable.h"
#include "inet/routing/base/RoutingProtocolBase.h"
#include "inet/transportlayer/udp/UdpHeader_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include <stdlib.h>
#include "../cryptopp850/aes.h"

using namespace std;
using namespace inet;

class SybilSecure: public GpsrSecure {
public:
    SybilSecure();
    virtual ~SybilSecure();
protected:
    virtual void processBeaconTimer() override;
    const Ptr<GpsrBeacon> createBeaconSybil(const char* addressStr);
    L3AddressResolver resolver = L3AddressResolver();
};

#endif /* GPSRBASE_SYBILSECURE_H_ */
