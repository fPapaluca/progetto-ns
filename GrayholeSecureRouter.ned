//
// This program is property of its copyright holder. All rights reserved.
//

import inet.node.inet.AdhocHost;
import gpsrbase.*;

module GrayholeSecureRouter extends AdhocHost
{
    submodules:
        grayholeSecure: GrayholeSecure {
            parameters:
                @display("p=825,226");
        }
    connections:
        grayholeSecure.ipOut --> tn.in++;
        grayholeSecure.ipIn <-- tn.out++;
}

