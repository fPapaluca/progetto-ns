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

import inet.node.gpsr.GpsrRouter;
import inet.*.*.IntegratedMultiVisualizer;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.*.*.UnitDiskRadioMedium;
import inet.*.*.StandardHost;

network FanetGpsr
{
    parameters:
        int numHosts;
    submodules:
        host[numHosts]: GpsrRouter {
            @display("i=misc/drone;p=0,0");
        }
        visualizer: IntegratedMultiVisualizer {
            @display("p=48.88,176.72");
        }
        radioMedium: UnitDiskRadioMedium {
            @display("p=100,100");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=100,300");
        }
}
