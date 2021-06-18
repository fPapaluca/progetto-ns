//
// This program is property of its copyright holder. All rights reserved.
//

import inet.node.inet.AdhocHost;
import gpsrbase.*;

module SybilRouter extends AdhocHost
{
    submodules:
        sybil: Sybil {
            parameters:
                @display("p=825,226");
        }
    connections:
        sybil.ipOut --> tn.in++;
        sybil.ipIn <-- tn.out++;
}

