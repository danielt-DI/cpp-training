#include <cstdio>

//
// Created by daniel on 20/04/2022.
//
int main(int argc, char *argv[]) {
    char c;

    if (argc != 1) {
        return -1;
    }

    FILE* f = fopen("test_file_deny.txt", "r");

    c = fgetc(f);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(f);
    }

    fclose(f);

    return 0;
}
