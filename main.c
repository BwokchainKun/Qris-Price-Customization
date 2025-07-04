#include <stdio.h>
#include <string.h>

char* getPriceAndModified(int price, char *priceTagIdentifer, char *QrisData) {
    int count = 0;
    int original_price = price;
    if (price == 0) {
        count = 1;
    }
    while (price > 0) {
        price /= 10;
        count++;
    }
    // Assign the price tag identifier with the format "54%02d%d" to priceTagIdentifer
    sprintf(priceTagIdentifer, "54%02d%d", count, original_price);
    // result will be like "5415000" for price Rp15000
    // 54 is the tag for transaction amount + Digit count of the price + the price itself

    // This line using specific currency code for IDR 360. 
    // 53 for tag structure + the digit of the currency code(03) + IDR CODE (360)
    // check other codes currency https://www.iban.com/currency-codes
    char CurrencyCode[] = "5303360";

    // find currency code in QrisData
    char *currencyCodePosition = strstr(QrisData, CurrencyCode);
    int finalPosition= 0;

    if (currencyCodePosition != NULL) {
        finalPosition = currencyCodePosition - QrisData + strlen(CurrencyCode);
    } else {
        finalPosition = -1; // Currency code not found
    }

    // Combine the price tag identifier with the currency code in QrisData
    static char QrisDataModified[2000] = "";

    strncpy(QrisDataModified, QrisData, finalPosition);
    QrisDataModified[finalPosition] = '\0';
    strcat(QrisDataModified, priceTagIdentifer);
    strcat(QrisDataModified, QrisData + finalPosition);

    // Remove old CRC properly
    char *crcPos = strstr(QrisDataModified, "6304");
    if (crcPos) {
        *crcPos = '\0';
    }

    return QrisDataModified;
}

char* countCRC(char *QrisDataModified) {
    // Add the "6304" tag before calculating the CRC, as required by QRIS specification
    char tempWithTag[2500];
    sprintf(tempWithTag, "%s6304", QrisDataModified);

    // Calculate CRC-CCITT-FALSE (polynomial 0x1021, initial value 0xFFFF)
    unsigned short crc = 0xFFFF;
    for (int i = 0; i < strlen(tempWithTag); i++) {
        crc ^= ((unsigned short)tempWithTag[i]) << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    crc &= 0xFFFF;

    // Convert CRC to a 4-digit uppercase hexadecimal string
    char crcHex[5];
    sprintf(crcHex, "%04X", crc);

    // Create the final QR string by appending "6304" tag and CRC value
    static char finalQrisData[2500] = "";
    strcpy(finalQrisData, QrisDataModified);
    strcat(finalQrisData, "6304");
    strcat(finalQrisData, crcHex);

    return finalQrisData;
}


int main() {
    char QrisData[] = "00020101021126710019ID.CO.CIMBNIAGA.WWW011893600022000068608102150000081009205200303UMI51450015ID.OR.QRNPG.WWW0215ID10243216249410303UMI5204481253033605802ID5923MBL342580*AGEN BLBMRI-36010PANDEGLANG61054225262120708M515280163040B3E";
    int price = 13000;
    char priceTagIdentifer[20];
    char *modifiedQrisData = countCRC(getPriceAndModified(price, priceTagIdentifer, QrisData));
    printf("%s\n", modifiedQrisData);
    return 0;
}

// QRIS GLOBAL TAG STRUCTURE
// 00   : Payload format indicator
// 01   : Point of initiation method (static/dynamic)
// 02-51 : Merchant account info (multiple sub-tags)
// 52   : Merchant category code
// 53   : Currency (360 = IDR)
// 54   : Transaction amount  ← nominal
// 55   : Tip
// 56   : Convenience fee
// 58   : Country code
// 59   : Merchant name
// 60   : Merchant city
// ...
// 62   : Additional data (ex: bill number)
// 63   : CRC (checksum)