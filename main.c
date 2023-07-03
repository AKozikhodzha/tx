#include"main.h"

struct Message {
    int AD;
    int ID;
    char* DATA;
};

int main(void)
{

    char str[] = "Hello world!";
    int length = strlen(str);
    int size = sizeof(str) / sizeof(str[0]); // размер входного массива

    char characters[length];
    
    for (int i = 0; i < length; i++) {
        characters[i] = str[i];
    }
    
    char one[length];
    replace_with_ascii(characters, length, one);
    
    printf("\nЗамененные символы на ASCII-коды:\n");
    for (int i = 0; i < length; i++) {
        printf("%d ", one[i]);
    }
    printf("\n");

    int counter = 0; // счетчик для записи преобразованных символов в массив
    char hexCode[size*2];

    to_base_ascii(one, hexCode);

    printf("\n16-ричные коды символов:\n");
    for (int i = 0; i < length; i++) {
        printf("%02x ", characters[i]);
    }
    printf("\n");

    printf("%s\n", hexCode); // выводим преобразованные символы

    int array_length = length / 2; // длина результирующего массива
    char result[array_length][3]; // создаем массив с элементами по 2 символа и дополнительным символом для нулевого завершения строки

    // Разделяем строку на элементы массива
    for (int i = 0; i < length; i += 2) {
        strncpy(result[i / 2], hexCode + i, 2);
        result[i / 2][2] = '\0'; // добавляем нулевой символ для завершения строки
    }
     // Выводим результат
    for (int i = 0; i < length; i++) {
        printf("%s ", result[i]);
    }
    printf("\n");
    
    struct Message message;
    message.AD = 21;
    message.ID = 2;
    *message.DATA=result;

    printf("AD: %d\n", message.AD);
    printf("ID: %d\n", message.ID);
    printf("DATA:\n");
    printf("%s\n", message.DATA);
    for (int i = 0; i < length; i++) {
        printf("%d ", message.DATA[i]);
    }
    printf("\n");

    return 0;
}
