/* makefont.c */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_OUT_FILE    "font.c"
#define DEFAULT_FONT_DATA   "hankaku"

struct ARGUMENTS {
    int32_t SourceNumber;       /* if it is not 1, program will occur an error */
    uint8_t *SourceFileName;
    uint8_t *OutFileName;
    uint8_t *FontDataName;
};

/* main function
 * arguments:
 *      argc - the number of arguments.
 *             No.0 is used to store this command name,
 *             so not used.
 *      argv - A argument type of (char **),
 *             the pointer to a pointer table of arguments's strings.
 */
int main(int argc, char **argv)
{
    int32_t count;
    struct ARGUMENTS arg = { 0, (uint8_t *) NULL, (uint8_t *) NULL, (uint8_t *) NULL };

    /* set the number of some options's arguments which is a member of ARGUMENTS */
    for (count = 1; count <= 6; count++) {  /* the maximum of number of arguments if 6 */
        if (*argv[count] == '-') {
            if (*(argv[count] + 1) == 'o') {
                arg.OutFileName  = (uint8_t *) argv[count + 1];
            } else if (*(argv[count] + 1) == 'n') {
                arg.FontDataName = (uint8_t *) argv[count + 1];
            } else {
                fprintf(stderr,
                        "hankaku2asm: [error] Unknown option \"-%c\" \n", *(argv[count] + 1));
                exit(EXIT_FAILURE);
            }
        } else {
            arg.SourceFileName = argv[count];
            arg.SourceNumber++;
        }
    }
    
    /* call hankaku2asm() */
    if (arg.SourceNumber < 1) {
        fprintf(stderr,
                "hankaku2asm: [error] No input file\n");
        exit(EXIT_FAILURE);
    } else if (arg.SourceNumber > 1) {
        fprintf(stderr,
                "hankaku2asm: [error] Input file is must be one\n");
        exit(EXIT_FAILURE);
    } else {        /* arg.SourceNumber == 1 */
        if (arg.OutFileName == (uint8_t *) NULL) {
            if (arg.FontDataName == (uint8_t *) NULL) {
                /* the default */
                hankaku2asm(arg.SourceFileName, DEFAULT_OUT_FILE, DEFAULT_FONT_DATA);
            } else {
                hankaku2asm(arg.SourceFileName, DEFAULT_OUT_FILE, arg.FontDataName);
            }
        } else {
            if (arg.FontDataName == (uint8_t *) NULL) {
                hankaku2asm(arg.SourceFileName, arg.OutFileName, DEFAULT_FONT_DATA);
            } else {
                hankaku2asm(arg.SourceFileName, arg.OutFileName, arg.FontDataName);
            }
        }
    }

    return 0;
}

int32_t hankaku2asm(uint8_t *in, uint8_t *out, uint8_t *FontDataName)
{
    /* combine file */
    FILE *infile, *outfile;
    uint8_t readed_by_infile[1024];
    int32_t local_counter, general_counter;

    uint8_t *source_code_no_comments, c_source;

    if (!(source_code_no_comments = (uint8_t *) malloc(100000))) {
        fprintf(stderr,
                "hankaku2asm: [error] Mamory arocating error\n");
        exit(EXIT_FAILURE);
    }

    if (!(infile = fopen(in, "r")) || !(outfile = fopen(out, "w"))) {
        fprintf(stderr, "hankaku2asm: [error] File open error\n");
        exit(EXIT_FAILURE);
    } else {
        general_counter = 0;
        for (;;) {
            if (!fgets(readed_by_infile, sizeof(readed_by_infile) - 1, infile)) {
                if (feof(infile)) {
                    /* if suceed in read and removing comments */
                    fwrite((c_source = translate(FontDataName)), 1, strlen(c_source), outfile);
                    goto exit;
                } else {
                    fprintf(stderr, "hankaku2asm: [error] An error occured in fgets()\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                /* eliminating comments */
                counter = 0;
                while (counter < 1024 &&
                        readed_by_infile[counter] != '#')
                    { counter++; general_counter++; }
                strncpy(source_code_no_comments, readed_by_infile, counter);
            }
        }
    }

exit:
    fclose(infile);
    fclose(outfile);
    free(source_code_no_comments);

    return 0;
}

uint8_t *translate(uint8_t *source, uint8_t *FontDataName)
{
    uint8_t *head;
    uint8_t buf[16];
    uint8_t *hex, *c_source, source_emilinated;
    uint8_t horizontal_font_data8;
    int32_t i, j, q;

    if (
            !(hex = (uint8_t *) malloc(100000))              ||
            !(c_source = (uint8_t *) malloc(100000))         ||
            !(source_emilinated = (uint8_t *) malloc(100000))
       ) {
        fprintf(stderr,
                "hankaku2asm: [error] Memory arocating error\n");
        exit(EXIT_FAILURE);
    }
    
    /* eliminating \n(<CR>) and space */
    for (i = 0, j = 0; i < 100000; i++) {
        if (source[i] != '\n' && source[i] != ' ') {
            source_emilinated[j] = source[i];
            j++;
        }
    }

    /* translating */
    for (i = 0; i < 256; i++) {
        for (j = 0; j < 16; j++) {
            for (q = 0; q < 8; q++) {
                if (source_emilinated[i + j + q] == '*')
                    horizontal_font_data8 |= (0x01 << (7 - q));
            }
            sprintf(buf, "0x%02x, ", horizontal_font_data8);
            strncat(hex, buf, 6);
        }
        strcat(hex, "\n    ");
    }

    /* combine head[] and foot[] and hex[] */
    sprintf(c_source, "%s%s%c%s%s",
            ".globl ", FontDataName, '\n', FontDataName, ":\n", hex);

    return c_source;
}

