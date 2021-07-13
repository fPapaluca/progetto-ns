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

#include "GpsrSecure.h"

using namespace std;
Define_Module(GpsrSecure);

void GpsrSecure::initialize(int stage)
{
    GpsrBase::initialize(stage);
    if(stage == 10){
        InitializeSec();

    }

}

void GpsrSecure::InitializeSec(){

    bool result = false;
    string  selfAddressStr = getSelfAddress().toIpv4().str();
    string  filename= "pk/"+selfAddressStr+".pem";
    srand(420);
    result = GeneratePrivateKey( CryptoPP::ASN1::secp160r1(), privateKey );

    result = GeneratePublicKey( privateKey, publicKey );

    string message = "Yoda said, Subaru Baracca. Don't try this at home.";
    string signature;

    result = SignMessage( privateKey, message, signature );

    result = VerifyMessage( publicKey, message, signature );

    SavePublicKey( filename, publicKey );
}

GpsrSecure::GpsrSecure() {


}

GpsrSecure::~GpsrSecure() {
    // TODO Auto-generated destructor stub
}




bool GpsrSecure::GeneratePrivateKey( const OID& oid, ECDSA<ECP, SHA1>::PrivateKey& key )
{
    AutoSeededRandomPool prng;

    key.Initialize( prng, oid );
    assert( key.Validate( prng, 3 ) );

    return key.Validate( prng, 3 );
}

bool GpsrSecure::GeneratePublicKey( const ECDSA<ECP, SHA1>::PrivateKey& privateKey, ECDSA<ECP, SHA1>::PublicKey& publicKey )
{
    AutoSeededRandomPool prng;

    // Sanity check
    assert( privateKey.Validate( prng, 3 ) );

    privateKey.MakePublicKey(publicKey);
    assert( publicKey.Validate( prng, 3 ) );

    return publicKey.Validate( prng, 3 );
}

void GpsrSecure::PrintPrivateKey( const ECDSA<ECP, SHA1>::PrivateKey& key )
{
    cout << endl;
    cout << "Private Exponent:" << endl;
    cout << " " << key.GetPrivateExponent() << endl;
}

void GpsrSecure::PrintPublicKey( const ECDSA<ECP, SHA1>::PublicKey& key )
{
    cout << endl;
    cout << "Public Element:" << endl;
    cout << " X: " << key.GetPublicElement().x << endl;
    cout << " Y: " << key.GetPublicElement().y << endl;
}

bool GpsrSecure::SignMessage( const ECDSA<ECP, SHA1>::PrivateKey& key, const string& message, string& signature )
{
    AutoSeededRandomPool prng;

    signature.erase();

    StringSource( message, true,
        new SignerFilter( prng,
            ECDSA<ECP,SHA1>::Signer(key),
            new StringSink( signature )
        ) // SignerFilter
    ); // StringSource

    return !signature.empty();
}

bool GpsrSecure::VerifyMessage( const ECDSA<ECP, SHA1>::PublicKey& key, const string& message, const string& signature )
{
    bool result = false;

    StringSource( signature+message, true,
        new SignatureVerificationFilter(
            ECDSA<ECP,SHA1>::Verifier(key),
            new ArraySink( (byte*)&result, sizeof(result) )
        ) // SignatureVerificationFilter
    );

    return result;
}

void GpsrSecure::SavePublicKey( const string& filename, const ECDSA<ECP, SHA1>::PublicKey& key )
{
    key.Save( FileSink( filename.c_str(), true /*binary*/ ).Ref() );
}

void GpsrSecure::LoadPublicKey( const string& filename, ECDSA<ECP, SHA1>::PublicKey& key )
{
    key.Load( FileSource( filename.c_str(), true /*pump all*/ ).Ref() );
}

const Ptr<GpsrBeacon> GpsrSecure::createBeacon()
{
    const auto& beacon = makeShared<GpsrBeacon>();
    beacon->setAddress(getSelfAddress());
    beacon->setPosition(mobility->getCurrentPosition());
    beacon->setSignature("noice");
    beacon->setChunkLength(B(getSelfAddress().getAddressType()->getAddressByteLength() + positionByteLength + strlen(beacon->getSignature())));
    return beacon;
}

void GpsrSecure::processBeacon(Packet *packet)
{
    const auto& beacon = packet->peekAtFront<GpsrBeacon>();
    cout << beacon->getSignature() << endl;
    EV_INFO << "Processing beacon: address = " << beacon->getAddress() << ", position = " << beacon->getPosition() << endl;
    neighborPositionTable.setPosition(beacon->getAddress(), beacon->getPosition());
    delete packet;
}


/*
Sybil::Sybil() {
    // TODO Auto-generated constructor stub
    srand(420);

    result = GeneratePrivateKey( CryptoPP::ASN1::secp160r1(), privateKey );

    result = GeneratePublicKey( privateKey, publicKey );

    string message = "Yoda said, Subaru Baracca. Don't try this at home.";
    string signature;

    result = SignMessage( privateKey, message, signature );

    result2 = VerifyMessage( publicKey, message, signature );

    cout << result2 << endl;





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
*/


