//
// ESP8266 Air Quality Sensor Example
//
#include"Esp8266AirQuality.h"

extern "C" {
    #include "user_interface.h"
}

Esp8266AirQuality airqualitysensor;

int current_quality =-1;
os_timer_t timer;         // Verwaltungsstruktur des Timers
int counter = 0;           // Argument für die Callback-Funktion
bool tickOccured = false;  // Flag, dass in der Callback-Funktion gesetzt wird
                           // Die aufwändigen und zeitintensiven Funktionen werden
                           // anhand dieses Flags im Hauptprogramm durchgeführt.


void timerCallback(void *pArg) {
//    *((int *) pArg) += 1;
    tickOccured = true;

    if (counter == 0) {
        airqualitysensor.last_vol=airqualitysensor.first_vol;
        airqualitysensor.first_vol=analogRead(A0);
        counter=10;
        airqualitysensor.timer_index=1;
    } else {
        counter--;
    }
}


void setup() {
    Serial.begin(9600);
    airqualitysensor.init(14);

    os_timer_setfn(&timer, timerCallback, &counter);
    os_timer_arm(&timer, 1000, true);
}

void loop() {
    current_quality=airqualitysensor.slope();
    if (current_quality >= 0) {
        if (current_quality==0)
            Serial.println("High pollution! Force signal active");
        else if (current_quality==1)
            Serial.println(":-(");
        else if (current_quality==2)
            Serial.println(":-|");
        else if (current_quality ==3)
            Serial.println(":-)");
    }

    if (tickOccured) {
        Serial.print(".");
//        Serial.print("Millis: "); Serial.println(millis());
//        Serial.print("Counter: "); Serial.println(counter);
        tickOccured = false;
    }
}
