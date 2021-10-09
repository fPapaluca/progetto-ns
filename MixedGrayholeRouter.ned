//
// This program is property of its copyright holder. All rights reserved.
//

import inet.node.inet.AdhocHost;
import gpsrbase.*;

module MixedGrayholeRouter extends AdhocHost
{
    submodules:
        mixedGrayhole: MixedGrayhole {
            parameters:
                @display("p=825,226");
        }
    connections:
        mixedGrayhole.ipOut --> tn.in++;
        mixedGrayhole.ipIn <-- tn.out++;
}

