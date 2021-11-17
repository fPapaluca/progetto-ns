/*
 * MixedSecure.cpp
 *
 *  Created on: 9 ott 2021
 *      Author: Francesco
 */

#include "MixedSecure.h"
#include <string>
#include "PromiscuousMode.h"

using namespace std;
Define_Module(MixedSecure);

void MixedSecure::initialize(int stage)
{
    GpsrGrayholeSecure::initialize(stage);
    if(stage == 10){
        InitializeSec();

    }

}

void MixedSecure::InitializeSec(){

    bool result = false;
    string  selfAddressStr = getSelfAddress().toIpv4().str();
    string  filename= "pk/"+selfAddressStr+".pem";
    srand(420);
    result = GeneratePrivateKey( CryptoPP::ASN1::secp256k1(), privateKey );

    result = GeneratePublicKey( privateKey, publicKey );
    SavePublicKey( filename, publicKey );
}

MixedSecure::MixedSecure() {


}

MixedSecure::~MixedSecure() {
    // TODO Auto-generated destructor stub
}




bool MixedSecure::GeneratePrivateKey( const OID& oid, ECDSA<ECP, SHA256>::PrivateKey& key )
{
    AutoSeededRandomPool prng;

    key.Initialize( prng, oid );
    assert( key.Validate( prng, 3 ) );

    return key.Validate( prng, 3 );
}

bool MixedSecure::GeneratePublicKey( const ECDSA<ECP, SHA256>::PrivateKey& privateKey, ECDSA<ECP, SHA256>::PublicKey& publicKey )
{
    AutoSeededRandomPool prng;

    // Sanity check
    assert( privateKey.Validate( prng, 3 ) );

    privateKey.MakePublicKey(publicKey);
    assert( publicKey.Validate( prng, 3 ) );

    return publicKey.Validate( prng, 3 );
}

void MixedSecure::PrintPrivateKey( const ECDSA<ECP, SHA256>::PrivateKey& key )
{
    cout << endl;
    cout << "Private Exponent:" << endl;
    cout << " " << key.GetPrivateExponent() << endl;
}

void MixedSecure::PrintPublicKey( const ECDSA<ECP, SHA256>::PublicKey& key )
{
    cout << endl;
    cout << "Public Element:" << endl;
    cout << " X: " << key.GetPublicElement().x << endl;
    cout << " Y: " << key.GetPublicElement().y << endl;
}

bool MixedSecure::SignMessage( const ECDSA<ECP, SHA256>::PrivateKey& key, const string& message, string& signature )
{
    AutoSeededRandomPool prng;

    signature.erase();

    StringSource( message, true,
        new SignerFilter( prng,
            ECDSA<ECP,SHA256>::Signer(key),
            new StringSink( signature )
        ) // SignerFilter
    ); // StringSource

    return !signature.empty();
}

bool MixedSecure::VerifyMessage( const ECDSA<ECP, SHA256>::PublicKey& key, const string& message, const string& signature )
{
    bool result = false;

    StringSource( signature+message, true,
        new SignatureVerificationFilter(
            ECDSA<ECP,SHA256>::Verifier(key),
            new ArraySink( (byte*)&result, sizeof(result) )
        ) // SignatureVerificationFilter
    );

    return result;
}

void MixedSecure::SavePublicKey( const string& filename, const ECDSA<ECP, SHA256>::PublicKey& key )
{
    key.Save( FileSink( filename.c_str(), true /*binary*/ ).Ref() );
}

void MixedSecure::LoadPublicKey( const string& filename, ECDSA<ECP, SHA256>::PublicKey& key )
{
    key.Load( FileSource( filename.c_str(), true /*pump all*/ ).Ref() );
}

const Ptr<GpsrBeacon> MixedSecure::createBeacon()
{
    bool result;
    const auto& beacon = makeShared<GpsrBeacon>();
    beacon->setAddress(getSelfAddress());
    beacon->setPosition(mobility->getCurrentPosition());
    string signature;
    string message = beacon->getAddress().str() + " " + beacon ->getPosition().str();
    result = SignMessage( privateKey, message, signature );
    beacon->setSignature(signature);
    beacon->setChunkLength(B(getSelfAddress().getAddressType()->getAddressByteLength() + positionByteLength + signature.length()));
    return beacon;
}

void MixedSecure::processBeacon(Packet *packet)
{
    ECDSA<ECP, SHA256>::PublicKey currentPublicKey;
    const auto& beacon = packet->peekAtFront<GpsrBeacon>();
    string  selfAddressStr = beacon->getAddress().toIpv4().str();
    string  filename= "pk/"+selfAddressStr+".pem";
    LoadPublicKey(filename, currentPublicKey);
    string message = beacon->getAddress().str() + " " + beacon ->getPosition().str();
    if(VerifyMessage(currentPublicKey,message,beacon->getSignature())){
        EV_INFO << "Processing beacon: address = " << beacon->getAddress() << ", position = " << beacon->getPosition() << endl;
        neighborPositionTable.setPosition(beacon->getAddress(), beacon->getPosition());
    }
    else{
        //Processing beacon con firma invalida
    }

    delete packet;
}

