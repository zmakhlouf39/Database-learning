#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_FILE  "database"
#define MAX_LINE 4096

/* Append "key,value" to the database file */
void db_set(const char *key, const char *value)
{
    FILE *f = fopen(DB_FILE, "a");
    if (!f) {
        perror("fopen");
        exit(1);
    }
    fprintf(f, "%s,%s\n", key, value);
    fclose(f);
}

/* Find the last line starting with "key," and copy its value into out.
   Returns 1 if found, 0 if not. */
int db_get(const char *key, char *out, size_t outsize)
{
    FILE *f = fopen(DB_FILE, "r");
    if (!f)
        return 0;

    char line[MAX_LINE];
    size_t klen = strlen(key);
    int found = 0;

    while (fgets(line, sizeof line, f)) {
        if (strncmp(line, key, klen) == 0 && line[klen] == ',') {
            char *val = line + klen + 1;
            val[strcspn(val, "\n")] = '\0';   /* strip trailing newline */
            strncpy(out, val, outsize - 1);
            out[outsize - 1] = '\0';
            found = 1;                          /* keep going: last match wins */
        }
    }

    fclose(f);
    return found;
}

int main(int argc, char *argv[])
{
    if (argc == 4 && strcmp(argv[1], "set") == 0) {
        db_set(argv[2], argv[3]);
    } else if (argc == 3 && strcmp(argv[1], "get") == 0) {
        char value[MAX_LINE];
        if (db_get(argv[2], value, sizeof value))
            puts(value);
        else
            return 1;
    } else {
        fprintf(stderr, "usage: %s set KEY VALUE | get KEY\n", argv[0]);
        return 2;
    }
    return 0;
}
