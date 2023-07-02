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

    char hexCode[length*2+1];

    asciiToHex(one, length, hexCode);

    printf("\n16-ричные коды символов:\n");
    for (int i = 0; i < length; i++) {
        printf("%02x ", characters[i]);
    }
    printf("\n");
    
    struct Message message;
    message.AD = 21;
    message.ID = 2;
    message.DATA=hexCode;

    printf("AD: %d\n", message.AD);
    printf("ID: %d\n", message.ID);
    printf("DATA:\n");
    for (int i = 0; i < length; i++) {
        printf("%d ", message.DATA[i]);
    }
    printf("\n");
    return 0;
}
