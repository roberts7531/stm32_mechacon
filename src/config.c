#include "util.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct searchmap_
{
    uint8_t* sourceptr;
    unsigned int sizeinbytes;
    const char* name;
    const char* altname;
}searchmap_t;

extern uint8_t MC_CARDKEY_MATERIAL_1[8];
extern uint8_t MC_CARDKEY_HASHKEY_1[16];
extern uint8_t MC_CARDKEY_MATERIAL_2[8];
extern uint8_t MC_CARDKEY_HASHKEY_2[16];
extern uint8_t MC_CHALLENGE_MATERIAL[8];
extern uint8_t MG_KBIT_MATERIAL[8];
extern uint8_t MG_KC_MATERIAL[8];
extern uint8_t MG_KBIT_MASTER_KEY[16];
extern uint8_t MG_KC_MASTER_KEY[16];

#define DECL_SRCH_ALT(x, z, a) {&x[0], z, #x, a}
#define DECL_SRCH(x, z) {&x[0], z, #x, NULL}
static searchmap_t searchmap[9] = {
    DECL_SRCH_ALT(MC_CARDKEY_MATERIAL_1, 8, "MG_CARDIV_0"),
    DECL_SRCH_ALT(MC_CARDKEY_HASHKEY_1, 16, "MG_CARDKEY_0"),
    DECL_SRCH_ALT(MC_CARDKEY_MATERIAL_2, 8, "MG_CARDIV2_0"),
    DECL_SRCH_ALT(MC_CARDKEY_HASHKEY_2, 16, "MG_CARDKEY2_0"),
    DECL_SRCH_ALT(MC_CHALLENGE_MATERIAL, 8, "MG_CHALLENGE_IV"),

    DECL_SRCH_ALT(MG_KBIT_MATERIAL, 8, "MG_KBIT_IV"),
    DECL_SRCH_ALT(MG_KC_MATERIAL, 8, "MG_KC_IV"),
    DECL_SRCH(MG_KBIT_MASTER_KEY, 16),
    DECL_SRCH(MG_KC_MASTER_KEY, 16),
};

extern int mg_retail;
extern int KeyChangeParam;
char* mgkeys = NULL;
int VERBOSE = 0;
int get_byte_array(int size, const char* origin, uint8_t* out) {
    if (strlen(origin) != (size*2)) {
        fprintf(stderr, "Invalid string len. expected %d chars\n", size*2);
        return EINVAL;
    }
    char* pos = origin;
    for (int count = 0; count < size; count++) {
        sscanf(pos, "%2hhx", &out[count]);
        //printf("%02X ", out[count]);
        pos += 2;
    }
    //printf("\t");
    return 0;
}

char fpath[256];
const char* get_globalconf() {
    #if defined(__linux__) || defined(__APPLE__)
    const char *xdg = getenv("XDG_CONFIG_HOME");
    if (xdg && xdg[0]) {
        snprintf(fpath, 128, "%s/ps3mca.ini", xdg);
    } else {
        snprintf(fpath, 128, "%s/.config/ps3mca.ini", getenv("HOME"));
    }
    #else
    snprintf(fpath, 128, "%s\\%s", getenv("USERPROFILE"), "ps3mca.ini");
    #endif
    return fpath;
}

int loadconfig(const char* source) {
    int ret = 0;
    struct ini_info *ini;
    ini = ini_load(source, 0);
    if (ini) {
        const char* t;
        mgkeys = ini_get(ini, "ps3mca", "mgkey");
        t = ini_get(ini, "ps3mca", "verbose");
        if (t) VERBOSE = atoi(t);
        if (VERBOSE > 0) printf("Using '%s' keys\n", mgkeys);


        t = ini_get(ini, mgkeys, "keychange_param");
        if (t) {
            KeyChangeParam = atoi(t);
        }
        for (int x = 0; x < 9; x++)
        {
            t = ini_get(ini, mgkeys, searchmap[x].name);
            if (!t && searchmap[x].altname != NULL) // if key not found & alt name avail
                t = ini_get(ini, mgkeys, searchmap[x].altname); // look for alt name
            if (!t) {
                COLOR(SRED);
                fprintf(stderr, "ERROR: missing key on config: '%s'\n", searchmap[x].name);
                COLOR(WHITE);
                ret = 47; //EFORMAT
                break;
            }
            if ((ret = get_byte_array(searchmap[x].sizeinbytes, t, searchmap[x].sourceptr)) != 0) break;
            if (VERBOSE > 10) {
                printf("%-23s: ", searchmap[x].name);
                for (int i = 0; i < searchmap[x].sizeinbytes; i++)
                    printf("%02X ", searchmap[x].sourceptr[i]);
                printf(" (%s)\n", t);
            }
        }
        ini_free(ini);
    } else {
        //fprintf(stderr, "Failed to load ini\n");
        ret = errno;
    }
    return ret;
}
