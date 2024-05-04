#include <stdio.h>
#include <string.h>
#include "src/list.h"
#include "src/parser.h"
#include "src/magic.h"

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

    fseek(date, 0, SEEK_END);
    size_t date_length = ftell(date);
    fseek(date, 0, SEEK_SET);

    char teamsBuffer[date_length + 1];
    memset(teamsBuffer, 0, sizeof(teamsBuffer));
    char cerinteBuffer[10];
    fread(cerinteBuffer, sizeof(char), sizeof(cerinteBuffer), cerinte);
    fread(teamsBuffer, sizeof(char), sizeof(teamsBuffer), date);


    handleRequirements(parseData(teamsBuffer), parseReq(cerinteBuffer), result);

    fclose(cerinte);
    fclose(date);
    fclose(result);
    return 0;
}
