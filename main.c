#include <stdio.h>

int getPriceAndModified(int price) {
    int count = 0;
    int original_price = price;
    if (price == 0) {
        count = 1;
    }
    while (price > 0) {
        price /= 10;
        count++;
    }
    return printf("54%02d%d", count, original_price);
}

void countCRC(int *count) {
    
}

void getStringAndModified(char str, int price) {
    
}



int main() {
    char QrisData;
    int price;
    getStringAndModified(QrisData, price);
    return 0;
}