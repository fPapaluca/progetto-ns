//
// This program is property of its copyright holder. All rights reserved.
//

import inet.node.inet.AdhocHost;
import gpsrbase.*;

module MixedSybilRouter extends AdhocHost
{
    submodules:
        mixedSybil: MixedSybil {
            parameters:
                @display("p=825,226");
        }
    connections:
        mixedSybil.ipOut --> tn.in++;
        mixedSybil.ipIn <-- tn.out++;
}

