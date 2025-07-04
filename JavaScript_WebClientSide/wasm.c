#include <stdio.h>
#include <string.h>

unsigned short calculateCRC(const unsigned char *data, size_t length) {
    unsigned short crc = 0xFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= ((unsigned short)data[i]) << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc & 0xFFFF;
}

char* generate_qris(const char *QrisData, int price) {
    static char priceTagIdentifier[20];
    static char QrisDataModified[2000];
    static char finalQrisData[2000];

    int count = (price == 0) ? 1 : 0;
    int original_price = price;

    while (price > 0) {
        price /= 10;
        count++;
    }

    sprintf(priceTagIdentifier, "54%02d%d", count, original_price);

    // This line uses a specific currency code for IDR 360. change as you need for other currency
    const char *CurrencyCode = "5303360";
    char *currencyCodePosition = strstr(QrisData, CurrencyCode);
    int finalPosition = 0;

    if (currencyCodePosition != NULL) {
        finalPosition = currencyCodePosition - QrisData + strlen(CurrencyCode);
    } else {
        strcpy(finalQrisData, "ERROR: Currency code not found.");
        return finalQrisData;
    }

    // Compose QRIS without CRC
    strncpy(QrisDataModified, QrisData, finalPosition);
    QrisDataModified[finalPosition] = '\0';
    strcat(QrisDataModified, priceTagIdentifier);
    strcat(QrisDataModified, QrisData + finalPosition);

    // Remove old CRC (remove last 4 char)
    char *crcPos = strstr(QrisDataModified, "6304");
    if (crcPos) {
        *crcPos = '\0';
    }

    // Calculate new CRC
    char tempWithTag[2100];
    char crcHex[5];
    sprintf(tempWithTag, "%s6304", QrisDataModified);
    unsigned short crc = calculateCRC((unsigned char*)tempWithTag, strlen(tempWithTag));

    // Convert CRC ke hex
    sprintf(crcHex, "%04X", crc & 0xFFFF);

    // Append CRC to the modified QRIS data
    strcpy(finalQrisData, QrisDataModified);
    strcat(finalQrisData, "6304");
    strcat(finalQrisData, crcHex);

    return finalQrisData;
}