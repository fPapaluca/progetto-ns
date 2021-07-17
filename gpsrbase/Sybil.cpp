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

#include "Sybil.h"


using namespace std;

Define_Module(Sybil);

Sybil::Sybil() {
    // TODO Auto-generated constructor stub
}

Sybil::~Sybil() {
    // TODO Auto-generated destructor stub
}

void Sybil::processBeaconTimer() {
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

const Ptr<GpsrBeacon> Sybil::createBeaconSybil(const char* addressStr)
{
    const auto& beacon = makeShared<GpsrBeacon>();
    beacon->setAddress(resolver.resolve(addressStr));
    beacon->setPosition(mobility->getCurrentPosition());
    beacon->setChunkLength(B(resolver.resolve(addressStr).getAddressType()->getAddressByteLength() + positionByteLength));
    return beacon;
}
