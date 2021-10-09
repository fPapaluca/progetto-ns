/*
 * MixedSecure.cpp
 *
 *  Created on: 9 ott 2021
 *      Author: Francesco
 */

#include "MixedGrayhole.h"
#include <string>
#include "PromiscuousMode.h"

using namespace std;
Define_Module(MixedGrayhole);

MixedGrayhole::MixedGrayhole() {
    // TODO Auto-generated constructor stub
}

MixedGrayhole::~MixedGrayhole() {
    // TODO Auto-generated destructor stub
}

INetfilter::IHook::Result MixedGrayhole::routeDatagram(Packet *datagram, GpsrOption *gpsrOption)
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
    double probability = ((double) rand() / (RAND_MAX));
    double discard_rate = 0.80;
    if (nextHop.isUnspecified() || probability < discard_rate) {
        //-cout << "scartato" << endl;
        EV_WARN << "No next hop found, dropping packet: source = " << source << ", destination = " << destination << endl;
        if (displayBubbles && hasGUI())
            getContainingNode(host)->bubble("No next hop found, dropping packet");
        return DROP;
    }
    else {
        //cout << "Source: " << previous_hop << " | Destination: " << getSelfAddress() << "| Packet name: "<< datagram->getName() << endl;
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
