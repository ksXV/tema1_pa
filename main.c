#include <stdio.h>
#define READ_ONLY "r"
#define CREATE_FILE "w"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "USAGE: lanParty [cerinte.in] [date.in] [result.out]\n");
        return -1;
    }

    FILE *cerinte = fopen(argv[1], READ_ONLY);
    if (cerinte == NULL) {
        fprintf(stderr, "ERROR: %s doesn't exists.", argv[1]);
        return -1;
    }

    FILE *date = fopen(argv[2], READ_ONLY);
    if (date == NULL) {
        fprintf(stderr, "ERROR: %s doesn't exists.", argv[2]);
        return -1;
    }

    FILE *result = fopen(argv[3], CREATE_FILE);
    if (result == NULL) {
        fprintf(stderr, "ERROR: Couldn't create %s.", argv[2]);
        return -1;
    } 

    //verifica daca pot citi din cerinte si date 
    char foobar[100];
    char foobar2[100];
    fread(foobar, sizeof(char), sizeof(foobar), cerinte);
    fread(foobar2, sizeof(char), sizeof(foobar2), date);

    /* puts(foobar); */
    /* puts(foobar2); */

    /* do stuff
     * do more stuff
     */
    


    fclose(cerinte);
    fclose(date);
    fclose(result);
    return 0;
}
