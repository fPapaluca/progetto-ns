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

#include "GpsrSecureBeacon.h"

Register_Class(GpsrSecureBeacon)

GpsrSecureBeacon::GpsrSecureBeacon() {
    // TODO Auto-generated constructor stub

}

GpsrSecureBeacon::~GpsrSecureBeacon() {
    // TODO Auto-generated destructor stub
}

const L3Address& GpsrSecureBeacon::getSignature() const {
    return this->signature;
}
void GpsrSecureBeacon::setSignature(const L3Address& signature){
    handleChange();
    this->signature = L3Address(signature);
}

void GpsrSecureBeacon::parsimPack(omnetpp::cCommBuffer *b) const
{
    GpsrBeacon::parsimPack(b);
    doParsimPacking(b,this->signature);
}

void GpsrSecureBeacon::parsimUnpack(omnetpp::cCommBuffer *b)
{
    GpsrBeacon::parsimUnpack(b);
    doParsimUnpacking(b,this->signature);
}

