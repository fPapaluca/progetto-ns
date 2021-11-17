/*
 * SybilSecure.cpp
 *
 *  Created on: 17 lug 2021
 *      Author: Francesco
 */

#include "SybilSecure.h"

Define_Module(SybilSecure);

SybilSecure::SybilSecure() {
    // TODO Auto-generated constructor stub

}

SybilSecure::~SybilSecure() {
    // TODO Auto-generated destructor stub
}

void SybilSecure::processBeaconTimer() {
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

const Ptr<GpsrBeacon> SybilSecure::createBeaconSybil(const char* addressStr)
{
    const auto& beacon = makeShared<GpsrBeacon>();
    beacon->setAddress(resolver.resolve(addressStr));
    beacon->setPosition(mobility->getCurrentPosition());
    string signature;
    string message = beacon->getAddress().str() + " " + beacon ->getPosition().str();
    SignMessage( privateKey, message, signature );
    beacon->setSignature(signature);
    beacon->setChunkLength(B(getSelfAddress().getAddressType()->getAddressByteLength() + positionByteLength + signature.length()));
    return beacon;
}
