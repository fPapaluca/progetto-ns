//
// This program is property of its copyright holder. All rights reserved.
//

import inet.node.inet.AdhocHost;
import gpsrbase.*;

module GpsrSecureRouter extends AdhocHost
{
    submodules:
        gpsrSecure: GpsrSecure {
            parameters:
                @display("p=825,226");
        }
    connections:
        gpsrSecure.ipOut --> tn.in++;
        gpsrSecure.ipIn <-- tn.out++;
}

