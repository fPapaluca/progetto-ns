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

#ifndef GPSRBASE_GPSRGRAYHOLESECURE_H_
#define GPSRBASE_GPSRGRAYHOLESECURE_H_

#include "GpsrBase.h"
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


class GpsrGrayholeSecure: public GpsrBase {
public:
    unordered_map<string,list<tuple<string,simtime_t>>> mappa_messaggi;
    unordered_map<string,int> mappa_num_non_inviati;
    unordered_map<string,int> mappa_num_inviati;
    unordered_map<string,simtime_t> mappa_simtime_t;
    GpsrGrayholeSecure();
    virtual ~GpsrGrayholeSecure();
    // handling beacons
    virtual Result routeDatagram(Packet *datagram, GpsrOption *gpsrOption) override;
    virtual void saveMessage(string dest, string msg);
    virtual void print_map(std::unordered_map<string,list<tuple<string,simtime_t>>> const &m);
    virtual void print_map2(std::unordered_map<string,int> const &m);
    virtual void deleteMessage(string dest, string msg, bool save);
    virtual void check_message();
    virtual L3Address findGreedyRoutingNextHop(const L3Address& destination, GpsrOption *gpsrOption) override;
    virtual L3Address findPerimeterRoutingNextHop(const L3Address& destination, GpsrOption *gpsrOption) override;
    virtual bool trustable(L3Address neighbourAddress);
    virtual string getUdpName(string all_string);
    virtual string getPreviousHop(string all_string);
    //virtual bool trustable2(L3Address neighbourAddress);
    virtual void initialize(int stage) override;
    virtual void finish() override;

};

#endif /* GPSRBASE_GPSRGRAYHOLESECURE_H_ */
