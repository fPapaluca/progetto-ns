/*
 * MixedSecure.h
 *
 *  Created on: 9 ott 2021
 *      Author: Francesco
 */

#ifndef GPSRBASE_MIXEDSECURE_H_
#define GPSRBASE_MIXEDSECURE_H_

#include "GpsrGrayholeSecure.h"
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

class MixedSecure: public GpsrGrayholeSecure {
public:
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    MixedSecure();
    virtual ~MixedSecure();
    virtual void initialize(int stage) override;
    virtual void InitializeSec();
    virtual bool GeneratePrivateKey( const OID& oid, ECDSA<ECP, SHA256>::PrivateKey& key );
    virtual bool GeneratePublicKey( const ECDSA<ECP, SHA256>::PrivateKey& privateKey, ECDSA<ECP, SHA256>::PublicKey& publicKey );
    virtual void PrintPrivateKey( const ECDSA<ECP, SHA256>::PrivateKey& key );
    virtual void PrintPublicKey( const ECDSA<ECP, SHA256>::PublicKey& key );
    virtual bool SignMessage( const ECDSA<ECP, SHA256>::PrivateKey& key, const string& message, string& signature );
    virtual bool VerifyMessage( const ECDSA<ECP, SHA256>::PublicKey& key, const string& message, const string& signature );
    virtual void SavePublicKey( const string& filename, const ECDSA<ECP, SHA256>::PublicKey& key );
    virtual void LoadPublicKey( const string& filename, ECDSA<ECP, SHA256>::PublicKey& key );
    virtual const Ptr<GpsrBeacon> createBeacon() override;
    virtual void processBeacon(Packet *packet) override;
};

#endif /* GPSRBASE_MIXEDSECURE_H_ */
