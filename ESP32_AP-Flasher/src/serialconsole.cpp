#include "serialconsole.h"

#include <Arduino.h>

#include "USB.h"

QueueHandle_t consoleCmdQueue;
TaskHandle_t consoleTaskHandle;
extern USBCDC USBSerial;

struct consoleCommand {
    uint8_t command = 0;
    uint8_t len = 0;
    uint8_t* data = nullptr;
};

void consoleStopTask() {
    if (consoleTaskHandle) vTaskDelete(consoleTaskHandle);
    consoleTaskHandle = NULL;
}

void consoleUartHandler(uint8_t* data, uint8_t len) {
    Serial.printf("in>");
    while (len--) {
        uint8_t usbbyte = *(data++);
        Serial.printf("%02X ", usbbyte);
    }
        Serial.printf("\n");
}

void consoleTask(void* parameter) {
    struct flasherCommand* cmd;
    USBSerial.write(0x1B);
    USBSerial.write(0x9B);
    USBSerial.print("9h");

    USBSerial.print("OHAI!!\n");
    USBSerial.write(0x1B);
    USBSerial.print("[?1003h");
    USBSerial.print("\e[?1003h\e[?1015h\e[?1006h");
    USBSerial.write(0x1B);
    USBSerial.print("[?1015h");
    USBSerial.write(0x1B);
    USBSerial.print("[?1006h");
    USBSerial.print("\e[?1000;1006;1015h"); // works
    USBSerial.print("\e[?1003;1015;1006h");
    while (true) {
        BaseType_t queuereceive = xQueueReceive(consoleCmdQueue, &cmd, 1500 / portTICK_PERIOD_MS);
        if (queuereceive == pdTRUE) {
        }
        USBSerial.println("testing.");
    }
}
