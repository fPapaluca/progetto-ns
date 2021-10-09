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
#include <string>
#include "PromiscuousMode.h"

using namespace std;
Define_Module(GpsrGrayholeSecure);


void GpsrGrayholeSecure::initialize(int stage)
{
    GpsrBase::initialize(stage);
    if(stage == 10){
        PromiscuousMode::getInstance().registerHost(this);
    }

}

GpsrGrayholeSecure::GpsrGrayholeSecure() {
    //PromiscuousMode::getInstance().registerHost(this);
}

GpsrGrayholeSecure::~GpsrGrayholeSecure() {
    // TODO Auto-generated destructor stub
}

string GpsrGrayholeSecure::getUdpName(string s) {
    string delimiter = "+";
    string token = s.substr(0, s.find(delimiter));
    return token;
}

string GpsrGrayholeSecure::getPreviousHop(string s) {
    string delimiter = "+";
    string token = s.substr(s.find(delimiter)+1, s.length());
    return token;
}

INetfilter::IHook::Result GpsrGrayholeSecure::routeDatagram(Packet *datagram, GpsrOption *gpsrOption)
{
    check_message();
    string all_string = datagram->getName();
    string datagram_name = getUdpName(all_string);
    string previous_hop = getPreviousHop(all_string);
    datagram->setName((datagram_name + "+" + getSelfAddress().str()).c_str());
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
        //cout << "Source: " << previous_hop << " | Destination: " << getSelfAddress() << "| Packet name: "<< datagram->getName() << endl;
        if(nextHop != destination && nextHop != getSelfAddress()){
            saveMessage(nextHop.str(), datagram_name);
        }
        if(previous_hop != datagram_name){
            PromiscuousMode::getInstance().mappa_host[previous_hop]->deleteMessage(getSelfAddress().str(), datagram_name, true);
        }
        EV_INFO << "Next hop found: source = " << source << ", destination = " << destination << ", nextHop: " << nextHop << endl;
        gpsrOption->setSenderAddress(getSelfAddress());
        auto interfaceEntry = CHK(interfaceTable->findInterfaceByName(outputInterface));
        datagram->addTagIfAbsent<InterfaceReq>()->setInterfaceId(interfaceEntry->getInterfaceId());
        return ACCEPT;
    }
}

/*
INetfilter::IHook::Result GpsrGrayholeSecure::routeDatagram(Packet *datagram, GpsrOption *gpsrOption)
{
    cout << PromiscuousMode::getInstance().mappa_host["10.0.0.21"]->getSelfAddress().str() << endl;
    check_message();
    string all_string = datagram->getName();
    string datagram_name = getUdpName(all_string);
    string previous_hop = getPreviousHop(all_string);
    cout << "previous hop " << previous_hop << endl;
    datagram->setName((datagram_name + "+" + getSelfAddress().str()).c_str());
    //print_map2(mappa_num_non_inviati);
    //string dataname = datagram->getName();
    //cout << dataname << endl;
    //datagram->setName((dataname + "+").c_str());
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
        if(!source.isUnspecified() && datagram_name != previous_hop){
            sendAck(createAck(datagram_name),L3Address(previous_hop.c_str()));
        }
        cout << "Source: " << source << " | Destination: " << getSelfAddress() << "| Packet name: "<< datagram->getName() << endl;
        //cout << "xxxx" << datagram->str() << endl;
        if(strncmp(datagram->getName(),"ACK",3)!=0){
            //cout << "Source:" << source << endl;
            if(nextHop.str() != destination.str()){
                saveMessage(nextHop.str(), datagram_name);
            }
        }
        EV_INFO << "Next hop found: source = " << source << ", destination = " << destination << ", nextHop: " << nextHop << endl;
        gpsrOption->setSenderAddress(getSelfAddress());
        auto interfaceEntry = CHK(interfaceTable->findInterfaceByName(outputInterface));
        datagram->addTagIfAbsent<InterfaceReq>()->setInterfaceId(interfaceEntry->getInterfaceId());
        return ACCEPT;
    }
}
*/

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

void GpsrGrayholeSecure::deleteMessage(string dest, string msg, bool save) {

    if (mappa_messaggi.count(dest) != 0) {
        // � presente tale destinataio neela mia mappa
        //cout << "entriamo in deletemessage" << endl;
        for ( list<tuple<string, simtime_t>>::iterator it = mappa_messaggi[dest].begin(); it != mappa_messaggi[dest].end(); it++) {
            if(strncmp(get<0>(*it).c_str(),msg.c_str(),32) == 0){
                //cout << get<0>(*it).c_str() << endl;
                //cout << msg.c_str() << endl;
                mappa_messaggi[dest].remove(*it);
                if(save){
                   // cout << simTime() - get<1>(*it) << endl;
                    if(mappa_num_inviati.count(dest)==0){
                        mappa_num_inviati[dest]=0;
                    }
                    mappa_num_inviati[dest] = mappa_num_inviati[dest] + 1;
                }
            }
            break;
        }
        //mappa_messaggi[dest].remove(msg);
        //print_map(mappa_messaggi);
    }
}

void GpsrGrayholeSecure::check_message(){
    simtime_t  now = simTime(); // seconds
    double timeout = 3; //


    for (auto const map_it: mappa_messaggi) {
        list<tuple<string,simtime_t>> list_copy = list<tuple<string,simtime_t>>(mappa_messaggi[map_it.first]);
        for (list<tuple<string, simtime_t>>::iterator list_it = list_copy.begin(); list_it != list_copy.end(); list_it++) {
            if( now - get<1>(*list_it) >= timeout){
                //cout << "timeout superato: " << now << " vs " << get<1>(*list_it) << endl;
                //cout << mappa_num_inviati[map_it.first] << " " << mappa_num_non_inviati[map_it.first] << endl;
                mappa_messaggi[map_it.first].remove(*list_it);
                deleteMessage(map_it.first,get<0>(*list_it), false);
                if(mappa_num_non_inviati.count(map_it.first)==0){
                    mappa_num_non_inviati[map_it.first]=0;
                }
                mappa_num_non_inviati[map_it.first]=mappa_num_non_inviati[map_it.first]+1;
                //cout << mappa_num_inviati[map_it.first] << " " << mappa_num_non_inviati[map_it.first] << endl;
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
            if(trustable(neighborAddress)){
                Coord neighborPosition = getNeighborPosition(neighborAddress);
                Coord intersection = computeIntersectionInsideLineSegments(perimeterRoutingStartPosition, destinationPosition, selfPosition, neighborPosition);
                if (std::isnan(intersection.x)) {
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
    //return mappa_num_non_inviati.count(neighbourAddress.str()) == 0 || mappa_num_non_inviati[neighbourAddress.str()] < 8;
    string neighbour = neighbourAddress.str();
    if(mappa_num_non_inviati.count(neighbour) == 0){
        mappa_num_non_inviati[neighbour] = 0;
    }
    if(mappa_num_inviati.count(neighbour) == 0){
        mappa_num_inviati[neighbour] = 0;
    }
    float s_count = mappa_num_inviati[neighbour];
    float f_count = mappa_num_non_inviati[neighbour];
    float trustness = (s_count + 1) / (s_count + f_count + 1);
    if(getSelfAddress().str() != "10.0.0.1"){
        //cout << this->getSelfAddress().str() << " --> "<< neighbour << ": " << trustness << endl;
    }
    float bound = 0.7;
    if(trustness >= bound){
        return true;
    }
    double probability = ((double) rand() / (RAND_MAX));
    return probability < 0.3;
}

void GpsrGrayholeSecure::finish(){
    int trust = 25;
    list<string> buoni;
    list<string> malevoli;
    for (int i = 0; i < trust; i++) {
        buoni.push_front("10.0.0." + to_string(i + 1));
    }
    for (int i = trust; i < 50; i++) {
        malevoli.push_front("10.0.0." + to_string(i + 1));
    }
    int matrix[2][2] = { { 0, 0 }, { 0, 0 } };
    for (auto const &pair : mappa_num_inviati) {
        auto neighbour = pair.first;
        float s_count = mappa_num_inviati[neighbour];
        float f_count = mappa_num_non_inviati[neighbour];
        float trustness = (s_count + 1) / (s_count + f_count + 1);
        int x = 0;
        int y = 0;
        if (trustness < 0.7) {
            y = 1;
        }
        if ((std::find(malevoli.begin(), malevoli.end(), neighbour)
                != malevoli.end())) {
            x = 1;
        }
        matrix[x][y] += 1;
    }
    cout << getSelfAddress().str() << " - tp " << matrix[0][0] << " - fn "
            << matrix[0][1] << " - fp " << matrix[1][0] << " - tn "
            << matrix[1][1] << endl;
    /* predetti -> buoni malevoli y
     * buoni tp fn
     * x malevoli fp tn
     *
     * */
}

/*
bool GpsrGrayholeSecure::trustable(L3Address neighbourAddress){
    string neighbour = neighbourAddress.str();
    if(mappa_num_non_inviati.count(neighbour) == 0){
        mappa_num_non_inviati[neighbour] = 0;
    }
    if(mappa_num_inviati.count(neighbour) == 0){
        mappa_num_inviati[neighbour] = 0;
    }
    if(mappa_vecchia_trust.count(neighbour) == 0){
        cout << "riga 1" << endl;
        tuple<float, simtime_t> elem (1.0,simTime());
        cout << "riga 2" << endl;
        mappa_vecchia_trust[neighbour] = elem;
        cout << "riga 3" << endl;
    }
    float s_count = mappa_num_inviati[neighbour];
    float f_count = mappa_num_non_inviati[neighbour];
    float trustness = (s_count + 1) / (s_count + f_count + 1);
    float alpha = 0.5;
    cout << "riga 4" << endl;
    float new_trust = (1-alpha)*get<0>(mappa_vecchia_trust[neighbour]) + alpha*trustness;
    cout << "riga 5" << endl;
    simtime_t update_time = 14;
    simtime_t now = simTime();
    if( (now - get<1>(mappa_vecchia_trust[neighbour])) > update_time ){
        cout << "riga 6" << endl;
        tuple<float, simtime_t> elem (new_trust, now);
        cout << "riga 7" << endl;
        mappa_vecchia_trust[neighbour] = elem;
        cout << "riga 8" << endl;
        mappa_num_inviati[neighbour] = 0;
        cout << "riga 9" << endl;
        mappa_num_non_inviati[neighbour] = 0;
        cout << "riga 10" << endl;
    }
    return new_trust > 0.75;
}
*/


