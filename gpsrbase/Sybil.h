//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef GPSRBASE_SYBIL_H_
#define GPSRBASE_SYBIL_H_

#include "GpsrBase.h"
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
using namespace CryptoPP;


class Sybil: public GpsrBase {
public:
    Sybil();
    virtual ~Sybil();
protected:
    virtual void processBeaconTimer() override;
    const Ptr<GpsrBeacon> createBeaconSybil(const char* addressStr);
    L3AddressResolver resolver = L3AddressResolver();

};

#endif /* GPSRBASE_SYBIL_H_ */
