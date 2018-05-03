#include <can.h>
#include <mcp2515.h>

#include <CanHacker.h>
#include <CanHackerLineReader.h>
#include <lib.h>

#include <SPI.h>

const int SPI_CS_PIN = 10;
const int INT_PIN = 3;


CanHackerLineReader *lineReader = NULL;
CanHacker *canHacker = NULL;

void handleError(const CanHacker::ERROR error);

void setup() {
    Serial.begin(115200);
    SPI.begin();

    Stream *interfaceStream = &Serial;
    Stream *debugStream = &Serial;
    
    
    canHacker = new CanHacker(interfaceStream, debugStream, SPI_CS_PIN);
    canHacker->enableLoopback();
    lineReader = new CanHackerLineReader(canHacker);
    
    pinMode(INT_PIN, INPUT);
}

void loop() {
    if (digitalRead(INT_PIN) == LOW) {
        CanHacker::ERROR error = canHacker->processInterrupt();
        handleError(error);
    }
    if (Serial.available()) {
          CanHacker::ERROR error = lineReader->process();
          handleError(error);
    }
}


void handleError(const CanHacker::ERROR error) {

    switch (error) {
        case CanHacker::ERROR_OK:
        case CanHacker::ERROR_UNKNOWN_COMMAND:
        case CanHacker::ERROR_NOT_CONNECTED:
        case CanHacker::ERROR_MCP2515_ERRIF:
        case CanHacker::ERROR_INVALID_COMMAND:
            return;

        default:
            break;
    }
  
    Serial.print("Failure (code ");
    Serial.print((int)error);
    Serial.println(")");
  
    while (1) {
        delay(2000);
    } ;
}
