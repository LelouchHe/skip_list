#include <stdio.h>
#include <string.h>

#include "skip_list.h"

struct kv_t {
    const char *k;
    const char *v;
} kv_infos[] = {
    {"1", "1"},
    {"3", "3"},
    {"5", "5"},
    {"7", "7"},
    {"2", "2"},
    {"4", "4"},
    {"6", "6"},
    {"8", "8"},
    {"9", "9"},
    {"0", "0"},
};

int kv_num = sizeof (kv_infos) / sizeof (kv_infos[0]);

int main() {
    struct skip_list_t *sl = msl_ini((msl_keycmp_t)strcmp);

    int i = 0;
    for (i = 0; i < kv_num; i++) {
        msl_set(sl, kv_infos[i].k, kv_infos[i].v);
    }

    printf("get\n");
    for (i = 0; i < kv_num; i++) {
        const char *v = (const char *)msl_get(sl, kv_infos[i].k);
        printf("key: %s\tvalue: %s\n", kv_infos[i].k, v == NULL ? "(null)" : v);
    }

    msl_erase(sl, "0");
    msl_erase(sl, "7");
    msl_erase(sl, "6");
    msl_erase(sl, "10");

    printf("iter\n");
    struct skip_list_iter_t it = msl_iter_next(sl, NULL);
    while (it.v != NULL) {
        const char *k = msl_iter_getk(&it);
        const char *v = msl_iter_getv(&it);

        printf("key: %s\tvalue: %s\n", k, v);

        it = msl_iter_next(sl, &it);
    }

    msl_debug(sl);

    msl_fini(sl);
    return 0;
}
