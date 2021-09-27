#ifndef GPSRBASE_PROMISCUOUSMODE_H_
#define GPSRBASE_PROMISCUOUSMODE_H_

#include "GpsrGrayholeSecure.h"
#include <unordered_map>
#include <string>

class PromiscuousMode
{
    public:
        static PromiscuousMode& getInstance()
        {
            static PromiscuousMode instance;

            return instance;
        }
    private:
        PromiscuousMode() {}

    public:
        unordered_map<string, GpsrGrayholeSecure*> mappa_host;
        PromiscuousMode(PromiscuousMode const&) = delete;
        void operator=(PromiscuousMode const&) = delete;
        void registerHost(GpsrGrayholeSecure* host);

};

#endif /* GPSRBASE_PROMISCUOUSMODE_H_ */
