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

import inet.*.*.IntegratedMultiVisualizer;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.*.*.UnitDiskRadioMedium;
import inet.*.*.StandardHost;
import inet.*.*.GpsrRouter;
import inet.common.geometry.common.OsgGeographicCoordinateSystem;


network FanetGpsrNormal
{
    parameters:
        int numHosts;
        int numSybils;
        int numGrayholes;
        @figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=host[20].app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);
        @figure[sndPkText](type=indicatorText; pos=380,40; anchor=w; font=,18; textFormat="packets sent: %g"; initialValue=0);
        @statistic[packetSent](source=host[0].app[0].packetSent; record=figure(count); targetFigure=sndPkText);
       
    submodules:
        host[numHosts]: GpsrRouter {
            @display("i=misc/drone;p=0,0");
        }

        sybil[numSybils]: SybilRouter {
            @display("i=misc/truck;p=0,0");
        }
        
        grayhole[numGrayholes]: GrayholeRouter {
            @display("i=misc/truck2;p=0,0");
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
        coordinateSystem: OsgGeographicCoordinateSystem {
            parameters:
                @display("p=150,600");
        } 
}


network FanetGpsrSecure
{
    parameters:
        int numHosts;
        int numSybils;
        //@figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        //@statistic[packetReceived](source=host[20].app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);
        //@figure[sndPkText](type=indicatorText; pos=380,40; anchor=w; font=,18; textFormat="packets sent: %g"; initialValue=0);
        //@statistic[packetSent](source=host[0].app[0].packetSent; record=figure(count); targetFigure=sndPkText);
        
        submodules:
        host[numHosts]: GpsrSecureRouter {
            @display("i=misc/drone;p=0,0");
        }

        sybil[numSybils]: SybilSecureRouter {
            @display("i=misc/truck;p=0,0");
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
        coordinateSystem: OsgGeographicCoordinateSystem {
            parameters:
                @display("p=150,600");
        } 
}


network FanetGpsrGrayholeSecure
{
    parameters:
        int numHosts;
        @figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=host[20].app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);
        @figure[sndPkText](type=indicatorText; pos=380,40; anchor=w; font=,18; textFormat="packets sent: %g"; initialValue=0);
        @statistic[packetSent](source=host[0].app[0].packetSent; record=figure(count); targetFigure=sndPkText);
        
        submodules:
        host[numHosts]: GpsrGrayholeSecureRouter {
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
        coordinateSystem: OsgGeographicCoordinateSystem {
            parameters:
                @display("p=150,600");
        } 
}

