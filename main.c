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
    static char QrisDataModified[1000] = "";

    strncpy(QrisDataModified, QrisData, finalPosition);
    QrisDataModified[finalPosition] = '\0';
    strcat(QrisDataModified, priceTagIdentifer);
    strcat(QrisDataModified, QrisData + finalPosition);

    // Remove old CRC
    int crcLength = strlen(QrisDataModified);
    QrisDataModified[crcLength - 4] = '\0'; // Remove last 4 characters (CRC)

    return QrisDataModified;
    
}

char* countCRC(char *QrisDataModified) {
    int crc = 0;
    int length = strlen(QrisDataModified);
    
    // Calculate CRC using the algorithm
    for (int i = 0; i < length; i++) {
        crc += QrisDataModified[i];
    }
    
    // Convert CRC to hexadecimal string
    char crcHex[5];
    sprintf(crcHex, "%04X", crc & 0xFFFF); // Ensure CRC is 4 characters long
    
    // Append CRC to the modified QRIS data
    static char finalQrisData[1000] = "";
    strcpy(finalQrisData, QrisDataModified);
    strcat(finalQrisData, crcHex);
    
    return finalQrisData;
    
}

void getStringAndModified(char str, int price) {
    
}



int main() {
    char QrisData[] = "00020101021126710019ID.CO.CIMBNIAGA.WWW011893600022000068608102150000081009205200303UMI51450015ID.OR.QRNPG.WWW0215ID10243216249410303UMI5204481253033605802ID5923MBL342580*AGEN BLBMRI-36010PANDEGLANG61054225262120708M515280163040B3E";
    int price = 1500;
    char priceTagIdentifer[20];
    char *modifiedQrisData = countCRC(getPriceAndModified(price, priceTagIdentifer, QrisData));
    printf("%s", modifiedQrisData);
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