#include "hashSet.h"



int main()
{
    htItem *ht[TABLE_SIZE];
    htInit(&ht);

    int len = htLen(&ht);
    printf("len: %d\n",len);

    insertItem(&ht, "123", "456");
    insertItem(&ht, "456", "789");

    char* a = getValue(&ht, "123");
    char* b = getValue(&ht, "456");
    printf("a: %s\n",a);
    printf("b: %s\n",b);

    printHashSet(&ht);

    int len1 = htLen(&ht);
    printf("len: %d\n",len1);

    deleteItem(&ht,"456");
    char* c = getValue(&ht, "456");
    printf("c: %s\n",c);

    int len2 = htLen(&ht);
    printf("len: %d\n",len2);

    return 0;
}
