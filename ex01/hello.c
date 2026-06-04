#include <stdio.h>

int main() {
    printf("Hello, World!\n");

    char input[256];
    printf("请输入内容：");
    fgets(input, sizeof(input), stdin);
    printf("你输入的内容是：%s", input);

    return 0;
}
