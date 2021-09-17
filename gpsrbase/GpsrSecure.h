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

#ifndef GPSRBASE_GPSRSECURE_H_
#define GPSRBASE_GPSRSECURE_H_

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




using namespace std;
using namespace inet;
using namespace CryptoPP;


class GpsrSecure: public GpsrBase {
public:
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    GpsrSecure();
    virtual ~GpsrSecure();
    virtual void initialize(int stage) override;
    void InitializeSec();
    bool GeneratePrivateKey( const OID& oid, ECDSA<ECP, SHA256>::PrivateKey& key );
    bool GeneratePublicKey( const ECDSA<ECP, SHA256>::PrivateKey& privateKey, ECDSA<ECP, SHA256>::PublicKey& publicKey );
    void PrintPrivateKey( const ECDSA<ECP, SHA256>::PrivateKey& key );
    void PrintPublicKey( const ECDSA<ECP, SHA256>::PublicKey& key );
    bool SignMessage( const ECDSA<ECP, SHA256>::PrivateKey& key, const string& message, string& signature );
    bool VerifyMessage( const ECDSA<ECP, SHA256>::PublicKey& key, const string& message, const string& signature );
    void SavePublicKey( const string& filename, const ECDSA<ECP, SHA256>::PublicKey& key );
    void LoadPublicKey( const string& filename, ECDSA<ECP, SHA256>::PublicKey& key );
    virtual const Ptr<GpsrBeacon> createBeacon() override;
    virtual void processBeacon(Packet *packet) override;
};

#endif /* GPSRBASE_GPSRSECURE_H_ */
