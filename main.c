#include "Ex4.h"
#include "heapFunc.cpp"
int main(int argc, char const *argv[])
{
    pstack root = NULL;
    for (int i = 0; i < 5; i++)
    {
        char *txt = (char *)my_malloc(sizeof(char) * 30);
        scanf("%s", txt);
        PUSH(&root, txt);
        my_free(txt);
    }

    while (!IsEmpty(root))
    {
        char *txt = POP(&root);
        printf("%s\n", txt);
        my_free(txt);
    }
    return 0;
}
