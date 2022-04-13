
#include <api/ArduinoAPI.h>
#include "../lib/WiFiNINA/src/utility/wifi_drv.h"

namespace Car {

    class CarController{
    public :
        void lockCar();
    public :
        void unlockCar();
    public :
        void configurePins();
    };

}
