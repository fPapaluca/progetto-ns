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

#ifndef GPSRBASE_GPSRSECUREBEACON_H_
#define GPSRBASE_GPSRSECUREBEACON_H_

#include <string>
#include "GpsrBase_m.h"

using namespace std;
using namespace inet;

class GpsrSecureBeacon: public inet::GpsrBeacon {
public:
    GpsrSecureBeacon();
    virtual ~GpsrSecureBeacon();
    L3Address signature;
    virtual const L3Address& getSignature() const;
    virtual L3Address& getSignatureForUpdate() { handleChange();return const_cast<L3Address&>(const_cast<GpsrSecureBeacon*>(this)->getSignature());}
    virtual void setSignature(const L3Address& signature);
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const GpsrSecureBeacon& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, GpsrSecureBeacon& obj) {obj.parsimUnpack(b);}

#endif /* GPSRBASE_GPSRSECUREBEACON_H_ */
