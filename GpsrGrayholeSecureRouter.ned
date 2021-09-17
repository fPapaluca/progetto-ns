//
// This program is property of its copyright holder. All rights reserved.
//

import inet.node.inet.AdhocHost;
import gpsrbase.*;

module GpsrGrayholeSecureRouter extends AdhocHost
{
    submodules:
        gpsrGrayholeSecure: GpsrGrayholeSecure {
            parameters:
                @display("p=825,226");
        }
    connections:
        gpsrGrayholeSecure.ipOut --> tn.in++;
        gpsrGrayholeSecure.ipIn <-- tn.out++;
}

