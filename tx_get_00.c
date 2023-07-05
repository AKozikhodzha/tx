#include<stdio.h>

void TxGet_write_data(char* data, size_t data_length, IsReady* flagMes, char* newData, size_t length) {
    int index = 0;

    for (int i = 0; i < data_length; i++) {
        if (data[i] == 54) {
            i++;

            if (i >= data_length || data[i] != 58)
                break;
        } else {
            flagMes=1;
            printf("flagMes = %d\n", flagMes);
            continue;
        }

        i++;

        while (i < length) {
            if (data[i] == 13 && data[i + 1] == 10)
                break;

            newData[index] = data[i];
            index++;

            i++;
        }
    }

    for (int j = 0; j < index; j++) {
        printf("%d ", newData[j]);
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    TxGet* flagMes;
    char data[] = {1, 54, 58, 8, 75, 2, 54, 55, 54, 3, 4, 13, 10, 44, 54, 5};
    char newData[sizeof(data)];
    size_t length = sizeof(newData) / sizeof(newData[0]);

    TxGet_write_data(data, sizeof(data), flagMes, newData, length);

    return 0;
}

