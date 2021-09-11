//
// This program is property of its copyright holder. All rights reserved.
//

import inet.node.inet.AdhocHost;
import gpsrbase.*;

module GrayholeRouter extends AdhocHost
{
    submodules:
        grayhole: Grayhole {
            parameters:
                @display("p=825,226");
        }
    connections:
        grayhole.ipOut --> tn.in++;
        grayhole.ipIn <-- tn.out++;
}

