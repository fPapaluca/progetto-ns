//
// This program is property of its copyright holder. All rights reserved.
//

import inet.node.inet.AdhocHost;
import gpsrbase.*;

module MixedSecureRouter extends AdhocHost
{
    submodules:
        mixedSecure: MixedSecure {
            parameters:
                @display("p=825,226");
        }
    connections:
        mixedSecure.ipOut --> tn.in++;
        mixedSecure.ipIn <-- tn.out++;
}

