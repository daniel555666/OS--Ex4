#include "Ex4.h"
int main(int argc, char const *argv[])
{
    pstack root = NULL;
    for (int i = 0; i < 5; i++)
    {
        char* txt = (char*)malloc(sizeof(char)*30);
        scanf("%s", txt);
        PUSH(&root, txt);
        free(txt);
    }
    
 
    while (!IsEmpty(root))
    {
        char* txt = POP(&root);
        printf("%s\n",txt);
        free(txt);
    }
    return 0;
}
