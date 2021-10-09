/*
 * MixedSecure.h
 *
 *  Created on: 9 ott 2021
 *      Author: Francesco
 */

#ifndef GPSRBASE_MIXEDGRAYHOLE_H_
#define GPSRBASE_MIXEDGRAYHOLE_H_

#include "MixedSecure.h"
#include <iostream>
#include <string>
#include <assert.h>
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
#include "../cryptopp850/files.h"
#include "../cryptopp850/eccrypto.h"
#include "../cryptopp850/osrng.h"
#include "../cryptopp850/integer.h"
#include "../cryptopp850/filters.h"
#include "../cryptopp850/oids.h"

#include <algorithm>

#include "inet/common/INETUtils.h"
#include "inet/common/IProtocolRegistrationListener.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/networklayer/common/HopLimitTag_m.h"
#include "inet/networklayer/common/IpProtocolId_m.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/networklayer/common/L3Tools.h"
#include "inet/networklayer/common/NextHopAddressTag_m.h"
#include "inet/networklayer/contract/IInterfaceTable.h"

#ifdef WITH_IPv4
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#endif

#ifdef WITH_IPv6
#include "inet/networklayer/ipv6/Ipv6ExtensionHeaders_m.h"
#include "inet/networklayer/ipv6/Ipv6InterfaceData.h"
#endif

#ifdef WITH_NEXTHOP
#include "inet/networklayer/nexthop/NextHopForwardingHeader_m.h"
#endif

#include <unordered_map>
#include <tuple>
#include <stdlib.h>




using namespace std;
using namespace inet;
using namespace CryptoPP;

class MixedGrayhole: public MixedSecure {
public:
    MixedGrayhole();
    virtual ~MixedGrayhole();
    virtual Result routeDatagram(Packet *datagram, GpsrOption *gpsrOption) override;
    virtual bool trustable(L3Address neighbourAddress) override {return true;}
};

#endif /* GPSRBASE_MIXED_H_ */
