#include<stdio.h>
#include<stdlib.h>
#include <string.h>

void replace_with_ascii(char* arr, int length, char* one) {
    for (int i = 0; i < length; i++) {
        one[i] = (int)arr[i];
    }
}

void asciiToHex(char* asciiCodes, int size, char* hexResult) {
    for(int i = 0; i < size; i++) {
        sprintf(hexResult + (i * 2), "%02X", asciiCodes[i]);
    }
}

void to_base_ascii(const char *input, char *result) {
    char base_range[] = "0123456789ABCDEF";
    int size = strlen(input);

    for (int i = 0; i < size; i++) {
        int value = input[i];
        int p = 0;

        if (value < 0) {
            value += 256; // добавляем 256 для поддержки отрицательных значений символов ASCII
        }

        do {
            result[i * 2 + p++] = base_range[value % 16];
            value /= 16;
        } while (value > 0);

        // Добавляем ведущий ноль, если необходимо
        if (p < 2) {
            result[i * 2 + p++] = '0';
        }
        
        // Развернуть результат
        for (int j = 0; j < p / 2; j++) {
            char temp = result[i * 2 + j];
            result[i * 2 + j] = result[i * 2 + p - j - 1];
            result[i * 2 + p - j - 1] = temp;
        }
    }
}