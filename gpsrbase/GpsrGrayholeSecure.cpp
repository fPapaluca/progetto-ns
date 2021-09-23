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

#include "GpsrGrayholeSecure.h"

using namespace std;
Define_Module(GpsrGrayholeSecure);


const Ptr<GpsrBeacon> GpsrGrayholeSecure::createAck(string name) ////////////////////////////////////////////////////////////////////////////
{
    const auto& beacon = makeShared<GpsrBeacon>();
    beacon->setAddress(getSelfAddress());
    beacon->setPosition(mobility->getCurrentPosition());
    beacon->setSignature(name.c_str());
    beacon->setChunkLength(B(getSelfAddress().getAddressType()->getAddressByteLength() + positionByteLength + name.length()));
    return beacon;
}

void GpsrGrayholeSecure::sendAck(const Ptr<GpsrBeacon>& beacon, const L3Address& address)
{
    EV_INFO << "Sending beacon: address = " << beacon->getAddress() << ", position = " << beacon->getPosition() << endl;
    Packet *udpPacket = new Packet("ACK");
    udpPacket->insertAtBack(beacon);
    auto udpHeader = makeShared<UdpHeader>();
    udpHeader->setSourcePort(GPSR_UDP_PORT);
    udpHeader->setDestinationPort(GPSR_UDP_PORT);
    udpHeader->setCrcMode(CRC_DISABLED);
    udpPacket->insertAtFront(udpHeader);
    auto addresses = udpPacket->addTag<L3AddressReq>();
    addresses->setSrcAddress(getSelfAddress());
    addresses->setDestAddress(address);
    udpPacket->addTag<HopLimitReq>()->setHopLimit(255);
    udpPacket->addTag<PacketProtocolTag>()->setProtocol(&Protocol::manet);
    udpPacket->addTag<DispatchProtocolReq>()->setProtocol(addressType->getNetworkProtocol());
    sendUdpPacket(udpPacket);
    cout << "ACK inviato" << endl;
}

void GpsrGrayholeSecure::processBeacon(Packet *packet)
{
    const auto& beacon = packet->peekAtFront<GpsrBeacon>();
    //cout << packet->getName() << endl;
    if(strncmp(packet->getName(),"ACK",3)==0){
        if(strncmp("(inet::Packet)ACK",beacon->getSignature().c_str(),17)==0){
            delete packet;
            return;
        }
        cout << "----" << beacon->getSignature() << " ----" << endl;
    }
    else{
        EV_INFO << "Processing beacon: address = " << beacon->getAddress() << ", position = " << beacon->getPosition() << endl;
        neighborPositionTable.setPosition(beacon->getAddress(), beacon->getPosition());
    }
    delete packet;
}


GpsrGrayholeSecure::GpsrGrayholeSecure() {


}

GpsrGrayholeSecure::~GpsrGrayholeSecure() {
    // TODO Auto-generated destructor stub
}

INetfilter::IHook::Result GpsrGrayholeSecure::routeDatagram(Packet *datagram, GpsrOption *gpsrOption)
{
    const auto& networkHeader = getNetworkProtocolHeader(datagram);
    const L3Address& source = networkHeader->getSourceAddress();
    const L3Address& destination = networkHeader->getDestinationAddress();
    EV_INFO << "Finding next hop: source = " << source << ", destination = " << destination << endl;
    auto nextHop = findNextHop(destination, gpsrOption);
    datagram->addTagIfAbsent<NextHopAddressReq>()->setNextHopAddress(nextHop);
    if (nextHop.isUnspecified()) {
        EV_WARN << "No next hop found, dropping packet: source = " << source << ", destination = " << destination << endl;
        if (displayBubbles && hasGUI())
            getContainingNode(host)->bubble("No next hop found, dropping packet");
        return DROP;
    }
    else {
        sendAck(createAck(datagram->str()),source);
        cout << "xxxx" << datagram->str() << endl;
        if(strncmp(datagram->getName(),"ACK",3)!=0){
            saveMessage(destination.str(), datagram->str());
            deleteMessage(destination.str(), datagram->str());
        }
        EV_INFO << "Next hop found: source = " << source << ", destination = " << destination << ", nextHop: " << nextHop << endl;
        gpsrOption->setSenderAddress(getSelfAddress());
        auto interfaceEntry = CHK(interfaceTable->findInterfaceByName(outputInterface));
        datagram->addTagIfAbsent<InterfaceReq>()->setInterfaceId(interfaceEntry->getInterfaceId());
        return ACCEPT;
    }
}

void GpsrGrayholeSecure::saveMessage(string dest, string msg){

    if(mappa_messaggi.count(dest)==0){
        list<tuple<string,simtime_t>> lista;
        mappa_messaggi[dest] = lista;
    }
    tuple<string, simtime_t> elem (msg,simTime());
    mappa_messaggi[dest].push_back(elem);
    print_map(mappa_messaggi);
    if( simTime() > 2){
        cout << " Il tempo a Derangolandia e': " << simTime() <<endl;
    }
}

void GpsrGrayholeSecure::print_map(std::unordered_map<string,list<tuple<string,simtime_t>>> const &m)
{
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": ";// << pair.second << "}\n";
        for(auto const &t : pair.second) {
            std::cout << get<0>(t) << ", " << get<1>(t) <<";; ";
        }
        std::cout << "}\n";
    }
}

// ci serve per eliminare un messaggi quando arriva l'ack

void GpsrGrayholeSecure::deleteMessage(string dest, string msg) {

    if (mappa_messaggi.count(dest) != 0) {
        // è presente tale destinataio neela mia mappa

        for ( list<tuple<string, simtime_t>>::iterator it = mappa_messaggi[dest].begin(); it != mappa_messaggi[dest].end(); it++) {
            if(strcmp(get<0>(*it).c_str(),msg.c_str()) == 0){
                mappa_messaggi[dest].remove(*it);
            }
        }
        //mappa_messaggi[dest].remove(msg);
        print_map(mappa_messaggi);
    }
}

void GpsrGrayholeSecure::check_message(){
    simtime_t  now = simTime(); // seconds
    double timeout = 60; //

    for (auto const map_it: mappa_messaggi) {
        for (list<tuple<string, simtime_t>>::iterator list_it = mappa_messaggi[map_it.first].begin(); list_it != mappa_messaggi[map_it.first].end(); list_it++) {
            if( now - get<1>(*list_it) > timeout){
                mappa_messaggi[map_it.first].remove(*list_it);
                if(mappa_num_non_inviati.count(map_it.first)==0){
                    mappa_num_non_inviati[map_it.first]=0;
                }
                mappa_num_non_inviati[map_it.first]=mappa_num_non_inviati[map_it.first]+1;
            }
        }
    }
}



