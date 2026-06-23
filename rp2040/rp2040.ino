#include <Arduino.h>
#include "hardware/uart.h"

#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define UART_BAUD 9600
#define UART_ID uart0
#define PARITY UART_PARITY_NONE

#define POWER_PIN 2
#define RESET_PIN 4
#define BOOT0_PIN 5


const char MAGIC[] = {0xFE, 0xED, 0xF0, 0x0D};

void setup() {
  Serial.begin(115200);

	// Init UART
	Serial1.setTX(UART_TX_PIN);
  Serial1.setRX(UART_RX_PIN);
  Serial1.begin(UART_BAUD, SERIAL_8N1);

	// Init GPIOs
	pinMode(POWER_PIN, OUTPUT);
	pinMode(BOOT0_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT_PULLUP);

	digitalWrite(BOOT0_PIN, HIGH);

	digitalWrite(POWER_PIN, HIGH);

	while (Serial.read() == -1) {
    yield();
	}

	digitalWrite(POWER_PIN, LOW);

	while (digitalRead(RESET_PIN) == HIGH) {
		tight_loop_contents();
	}

	digitalWrite(POWER_PIN, HIGH);

	delay(42);

	digitalWrite(BOOT0_PIN, LOW);
	
	pinMode(RESET_PIN, OUTPUT);
	digitalWrite(RESET_PIN, 0);

	delay (42);
	
	pinMode(RESET_PIN, INPUT_PULLUP);

	wait_for_magic();

}

void loop() {
    int b = Serial1.read();

    if (b >= 0) {
        if (b < 0x10) Serial.print('0');
        Serial.print(b, HEX);
        Serial.print(' ');
    }
}

void wait_for_magic() {
    int matched = 0;

    while (matched < 4) {
        int b = Serial1.read();

        if (b < 0) {
            yield();
            continue;
        }

        if ((uint8_t)b == MAGIC[matched]) {
            matched++;
        } else {
            matched = 0;
        }
    }
}
