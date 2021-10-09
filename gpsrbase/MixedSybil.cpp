/*
 * MixedSecure.cpp
 *
 *  Created on: 9 ott 2021
 *      Author: Francesco
 */

#include "MixedSybil.h"
#include <string>
#include "PromiscuousMode.h"

using namespace std;
Define_Module(MixedSybil);

MixedSybil::MixedSybil() {
    // TODO Auto-generated constructor stub

}

MixedSybil::~MixedSybil() {
    // TODO Auto-generated destructor stub
}

void MixedSybil::processBeaconTimer() {
    EV_DEBUG << "Processing beacon timer" << endl;
    const char*  selfAddressStr = getSelfAddress().toIpv4().str().c_str();
    const L3Address selfAddress = getSelfAddress();
    if (!selfAddress.isUnspecified()) {
        sendBeacon(createBeaconSybil(("host["+to_string(rand() % 30)+"]").c_str()));
        storeSelfPositionInGlobalRegistry();
    }
    scheduleBeaconTimer();
    schedulePurgeNeighborsTimer();
}

const Ptr<GpsrBeacon> MixedSybil::createBeaconSybil(const char* addressStr)
{
    const auto& beacon = makeShared<GpsrBeacon>();
    beacon->setAddress(resolver.resolve(addressStr));
    beacon->setPosition(mobility->getCurrentPosition());
    string signature;
    string message = beacon->getAddress().str() + " " + beacon ->getPosition().str();
    //cout << "this is the message: " << message << endl ;
    SignMessage( privateKey, message, signature );
    //cout << "this is the signature in create beacon: "+ signature << endl ;
    //cout << "signature length " << signature.length() <<endl;
    beacon->setSignature(signature);
    //cout << "beacon->getSignature() length " << signature.length() <<endl;
    beacon->setChunkLength(B(getSelfAddress().getAddressType()->getAddressByteLength() + positionByteLength + signature.length()));
    return beacon;
}


