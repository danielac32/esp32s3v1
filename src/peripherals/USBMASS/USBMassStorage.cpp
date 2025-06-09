#include "USBMassStorage.h"

// Validación de hardware
#if !SOC_USB_OTG_SUPPORTED || ARDUINO_USB_MODE
#error Device does not support USB_OTG or native USB CDC/JTAG is selected
#endif

USBMassStorage::USBMassStorage(
    uint8_t clk,
    uint8_t cmd,
    uint8_t d0,
    uint8_t d1,
    uint8_t d2,
    uint8_t d3,
    bool oneBitMode
)
    : _clk(clk),
      _cmd(cmd),
      _d0(d0),
      _d1(d1),
      _d2(d2),
      _d3(d3),
      _oneBitMode(oneBitMode)
{
}

void USBMassStorage::begin() {
    // Iniciar comunicación serie
    Serial.println("Starting Serial");

    // Configurar y montar SD/MMC
    Serial.println("Mounting SDcard");
    SD_MMC.setPins(_clk, _cmd, _d0, _d1, _d2, _d3);
    if (!SD_MMC.begin("/sdcard", _oneBitMode)) {
        Serial.println("Mount Failed");
        return;
    }

    // Mostrar información de la tarjeta
    Serial.printf("Card Size: %lluMB\n", SD_MMC.totalBytes() / 1024 / 1024);
    Serial.printf("Sector: %d\tCount: %d\n", SD_MMC.sectorSize(), SD_MMC.numSectors());

    // Configurar USB MSC
    Serial.println("Initializing MSC");
    msc.vendorID("ESP32");
    msc.productID("USB_MSC");
    msc.productRevision("1.0");
    msc.onRead(onRead);
    msc.onWrite(onWrite);
    msc.onStartStop(onStartStop);
    msc.mediaPresent(true);
    msc.begin(SD_MMC.numSectors(), SD_MMC.sectorSize());

    // Iniciar USB
    Serial.println("Initializing USB");
    USB.begin();
    USB.onEvent(usbEventCallback);

    Serial.println("USB Mass Storage ready!");
    while(1){
        delay(10);
    }
}

int32_t USBMassStorage::onWrite(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    uint32_t secSize = SD_MMC.sectorSize();
    if (!secSize) return false;

    for (int x = 0; x < bufsize / secSize; x++) {
        uint8_t blkbuffer[secSize];
        memcpy(blkbuffer, buffer + secSize * x, secSize);
        if (!SD_MMC.writeRAW(blkbuffer, lba + x)) {
            return false;
        }
    }
    return bufsize;
}

int32_t USBMassStorage::onRead(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    uint32_t secSize = SD_MMC.sectorSize();
    if (!secSize) return false;

    for (int x = 0; x < bufsize / secSize; x++) {
        if (!SD_MMC.readRAW((uint8_t *)buffer + (x * secSize), lba + x)) {
            return false;
        }
    }
    return bufsize;
}

bool USBMassStorage::onStartStop(uint8_t power_condition, bool start, bool load_eject) {
    Serial.printf("Start/Stop power: %u\tstart: %d\teject: %d\n", power_condition, start, load_eject);
    return true;
}

void USBMassStorage::usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == ARDUINO_USB_EVENTS) {
        arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
        switch (event_id) {
            case ARDUINO_USB_STARTED_EVENT:
                Serial.println("USB PLUGGED");
                break;
            case ARDUINO_USB_STOPPED_EVENT:
                Serial.println("USB UNPLUGGED");
                break;
            case ARDUINO_USB_SUSPEND_EVENT:
                Serial.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en);
                break;
            case ARDUINO_USB_RESUME_EVENT:
                Serial.println("USB RESUMED");
                break;
            default:
                break;
        }
    }
}