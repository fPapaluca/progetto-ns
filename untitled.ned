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

// numOfHosts: 10
// parametric: true
// static:     false

import inet.node.gpsr.GpsrRouter;
import inet.*.*.IntegratedVisualizer;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.*.*.AdhocHost;
import inet.*.*.*.RadioMedium;
network Untitled
{
    parameters:
        int numHosts;
    submodules:
        host[numHosts]: AdhocHost
        {
            parameters:
                @display("r=,,#707070");
        }

        channelControl: RadioMedium
        {
            parameters:
                @display("p=60,50");
        }

        configurator: Ipv4NetworkConfigurator
        {
            @display("p=140,50");
        }
}
