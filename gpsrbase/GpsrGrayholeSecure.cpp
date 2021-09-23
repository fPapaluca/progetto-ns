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
    //cout << "ACK inviato" << endl;
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
        deleteMessage(beacon->getAddress().str(), beacon->getSignature());
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

    check_message();
    print_map2(mappa_num_non_inviati);

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
        //cout << "xxxx" << datagram->str() << endl;
        if(strncmp(datagram->getName(),"ACK",3)!=0){
            if(nextHop.str() != destination.str()){
                saveMessage(nextHop.str(), datagram->str());
            }
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
    //print_map(mappa_messaggi);
}

void GpsrGrayholeSecure::print_map(std::unordered_map<string,list<tuple<string,simtime_t>>> const &m)
{
    for (auto const &pair: m) {
        std::cout << "msg{" << pair.first << ": ";// << pair.second << "}\n";
        for(auto const &t : pair.second) {
            std::cout << get<0>(t) << ", " << get<1>(t) <<";; ";
        }
        std::cout << "}\n";
    }
}

void GpsrGrayholeSecure::print_map2(std::unordered_map<string,int> const &m)
{
    for (auto const &pair: m) {
        std::cout << "num{" << pair.first << ": " << pair.second << "}\n";
    }
}

// ci serve per eliminare un messaggi quando arriva l'ack

void GpsrGrayholeSecure::deleteMessage(string dest, string msg) {

    if (mappa_messaggi.count(dest) != 0) {
        // è presente tale destinataio neela mia mappa

        for ( list<tuple<string, simtime_t>>::iterator it = mappa_messaggi[dest].begin(); it != mappa_messaggi[dest].end(); it++) {
            if(strncmp(get<0>(*it).c_str(),msg.c_str(),32) == 0){
                mappa_messaggi[dest].remove(*it);
            }
            break;
        }
        //mappa_messaggi[dest].remove(msg);
        //print_map(mappa_messaggi);
    }
}

void GpsrGrayholeSecure::check_message(){
    simtime_t  now = simTime(); // seconds
    double timeout = 7; //


    for (auto const map_it: mappa_messaggi) {
        list<tuple<string,simtime_t>> list_copy = list<tuple<string,simtime_t>>(mappa_messaggi[map_it.first]);
        for (list<tuple<string, simtime_t>>::iterator list_it = list_copy.begin(); list_it != list_copy.end(); list_it++) {
            if( now - get<1>(*list_it) >= timeout){
                cout << "timeout superato: " << now << " vs " << get<1>(*list_it) << endl;
                mappa_messaggi[map_it.first].remove(*list_it);
                deleteMessage(map_it.first,get<0>(*list_it));
                if(mappa_num_non_inviati.count(map_it.first)==0){
                    mappa_num_non_inviati[map_it.first]=0;
                }
                mappa_num_non_inviati[map_it.first]=mappa_num_non_inviati[map_it.first]+1;
            }
        }
    }
}

L3Address GpsrGrayholeSecure::findGreedyRoutingNextHop(const L3Address& destination, GpsrOption *gpsrOption)
{
    EV_DEBUG << "Finding next hop using greedy routing: destination = " << destination << endl;
    L3Address selfAddress = getSelfAddress();
    Coord selfPosition = mobility->getCurrentPosition();
    Coord destinationPosition = gpsrOption->getDestinationPosition();
    double bestDistance = (destinationPosition - selfPosition).length();
    L3Address bestNeighbor;
    std::vector<L3Address> neighborAddresses = neighborPositionTable.getAddresses();
    for (auto& neighborAddress: neighborAddresses) {
        Coord neighborPosition = neighborPositionTable.getPosition(neighborAddress);
        double neighborDistance = (destinationPosition - neighborPosition).length();
        if (neighborDistance < bestDistance && trustable(neighborAddress)) {
            bestDistance = neighborDistance;
            bestNeighbor = neighborAddress;
        }
    }
    if (bestNeighbor.isUnspecified()) {
        EV_DEBUG << "Switching to perimeter routing: destination = " << destination << endl;
        if (displayBubbles && hasGUI())
            getContainingNode(host)->bubble("Switching to perimeter routing");
        gpsrOption->setRoutingMode(GPSR_PERIMETER_ROUTING);
        gpsrOption->setPerimeterRoutingStartPosition(selfPosition);
        gpsrOption->setPerimeterRoutingForwardPosition(selfPosition);
        gpsrOption->setCurrentFaceFirstSenderAddress(selfAddress);
        gpsrOption->setCurrentFaceFirstReceiverAddress(L3Address());
        return findPerimeterRoutingNextHop(destination, gpsrOption);
    }
    else
        return bestNeighbor;
}

L3Address GpsrGrayholeSecure::findPerimeterRoutingNextHop(const L3Address& destination, GpsrOption *gpsrOption)
{
    EV_DEBUG << "Finding next hop using perimeter routing: destination = " << destination << endl;
    L3Address selfAddress = getSelfAddress();
    Coord selfPosition = mobility->getCurrentPosition();
    Coord perimeterRoutingStartPosition = gpsrOption->getPerimeterRoutingStartPosition();
    Coord destinationPosition = gpsrOption->getDestinationPosition();
    double selfDistance = (destinationPosition - selfPosition).length();
    double perimeterRoutingStartDistance = (destinationPosition - perimeterRoutingStartPosition).length();
    if (selfDistance < perimeterRoutingStartDistance) {
        EV_DEBUG << "Switching to greedy routing: destination = " << destination << endl;
        if (displayBubbles && hasGUI())
            getContainingNode(host)->bubble("Switching to greedy routing");
        gpsrOption->setRoutingMode(GPSR_GREEDY_ROUTING);
        gpsrOption->setPerimeterRoutingStartPosition(Coord());
        gpsrOption->setPerimeterRoutingForwardPosition(Coord());
        gpsrOption->setCurrentFaceFirstSenderAddress(L3Address());
        gpsrOption->setCurrentFaceFirstReceiverAddress(L3Address());
        return findGreedyRoutingNextHop(destination, gpsrOption);
    }
    else {
        const L3Address& firstSenderAddress = gpsrOption->getCurrentFaceFirstSenderAddress();
        const L3Address& firstReceiverAddress = gpsrOption->getCurrentFaceFirstReceiverAddress();
        auto senderNeighborAddress = gpsrOption->getSenderAddress();
        auto neighborAngle = senderNeighborAddress.isUnspecified() ? getVectorAngle(destinationPosition - mobility->getCurrentPosition()) : getNeighborAngle(senderNeighborAddress);
        L3Address selectedNeighborAddress;
        std::vector<L3Address> neighborAddresses = getPlanarNeighborsCounterClockwise(neighborAngle);
        for (auto& neighborAddress : neighborAddresses) {
            Coord neighborPosition = getNeighborPosition(neighborAddress);
            Coord intersection = computeIntersectionInsideLineSegments(perimeterRoutingStartPosition, destinationPosition, selfPosition, neighborPosition);
            if (std::isnan(intersection.x) && trustable(neighborAddress)) {
                selectedNeighborAddress = neighborAddress;
                break;
            }
            else {
                EV_DEBUG << "Edge to next hop intersects: intersection = " << intersection << ", nextNeighbor = " << selectedNeighborAddress << ", firstSender = " << firstSenderAddress << ", firstReceiver = " << firstReceiverAddress << ", destination = " << destination << endl;
                gpsrOption->setCurrentFaceFirstSenderAddress(selfAddress);
                gpsrOption->setCurrentFaceFirstReceiverAddress(L3Address());
                gpsrOption->setPerimeterRoutingForwardPosition(intersection);
            }
        }
        if (selectedNeighborAddress.isUnspecified()) {
            EV_DEBUG << "No suitable planar graph neighbor found in perimeter routing: firstSender = " << firstSenderAddress << ", firstReceiver = " << firstReceiverAddress << ", destination = " << destination << endl;
            return L3Address();
        }
        else if (firstSenderAddress == selfAddress && firstReceiverAddress == selectedNeighborAddress) {
            EV_DEBUG << "End of perimeter reached: firstSender = " << firstSenderAddress << ", firstReceiver = " << firstReceiverAddress << ", destination = " << destination << endl;
            if (displayBubbles && hasGUI())
                getContainingNode(host)->bubble("End of perimeter reached");
            return L3Address();
        }
        else {
            if (gpsrOption->getCurrentFaceFirstReceiverAddress().isUnspecified())
                gpsrOption->setCurrentFaceFirstReceiverAddress(selectedNeighborAddress);
            return selectedNeighborAddress;
        }
    }
}

bool GpsrGrayholeSecure::trustable(L3Address neighbourAddress){
    return mappa_num_non_inviati.count(neighbourAddress.str()) == 0 || mappa_num_non_inviati[neighbourAddress.str()] < 8;
}



