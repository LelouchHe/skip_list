#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "skip_list.h"

/* 概率为5/10 */
#define RANDOM_MAX 10
#define RANDOM_THRESHOLD 5
#define MAX_HEIGHT 16

struct skip_list_node_t {
    const void *key;
    const void *value;

    struct skip_list_node_t *next[MAX_HEIGHT];
};

struct skip_list_t {
    struct skip_list_node_t head; /* 利用伪头简化处理 */

    int level; /* 当前的最大层数 */
    msl_keycmp_t keycmp;
};

struct skip_list_t *msl_ini(msl_keycmp_t keycmp) {
    struct skip_list_t *msl = (struct skip_list_t *)calloc(1, sizeof (struct skip_list_t));
    if (msl == NULL) {
        return NULL;
    }

    /* head/level已经被calloc清零 */
    msl->keycmp = keycmp;
    srand(time(NULL));

    return msl;
}

void msl_fini(struct skip_list_t *msl) {
    if (msl == NULL) {
        return;
    }

    struct skip_list_node_t *ph = &msl->head;
    while (ph->next[0] != NULL) {
        struct skip_list_node_t *next = ph->next[0]->next[0];
        free(ph->next[0]);
        ph->next[0] = next;
    }

    free(msl);
}

static struct skip_list_node_t *find_prev(struct skip_list_t *msl, const void *key, struct skip_list_node_t *prevs[]) {
    assert(msl != NULL);
    assert(key != NULL);

    struct skip_list_node_t *ph = &msl->head;
    int level;
    for (level = msl->level; level >= 0; level--) {
        while (ph->next[level] != NULL
            && msl->keycmp(ph->next[level]->key, key) < 0) {
            ph = ph->next[level];
        }
        if (prevs != NULL) {
            prevs[level] = ph;
        }
    }

    return ph;
}

static int pick_level() {
    int level = 0;
    while (rand() % RANDOM_MAX < RANDOM_THRESHOLD
        && level < MAX_HEIGHT - 1) {
        level++;
    }

    return level;
}

int msl_set(struct skip_list_t *msl, const void *key, const void *value) {
    if (msl == NULL || key == NULL) {
        return -1;
    }

    struct skip_list_node_t *prevs[MAX_HEIGHT];
    struct skip_list_node_t *prev = find_prev(msl, key, prevs);
    if (prev->next[0] != NULL && msl->keycmp(prev->next[0]->key, key) == 0) {
        prev->next[0]->value = value;
        return 0;
    }

    struct skip_list_node_t *node = (struct skip_list_node_t *)calloc(1, sizeof (struct skip_list_node_t));
    if (node == NULL) {
        return -1;
    }
    /* next被callco清零 */
    node->key = key;
    node->value = value;

    int level = pick_level();
    assert(level >= 0 && level < MAX_HEIGHT);

    while (msl->level < level) {
        msl->level++;
        prevs[msl->level] = &msl->head;
    }

    while (level >= 0) {
        node->next[level] = prevs[level]->next[level];
        prevs[level]->next[level] = node;

        level--;
    }

    return 0;
}

const void *msl_get(struct skip_list_t *msl, const void *key) {
    if (msl == NULL || key == NULL) {
        return NULL;
    }

    struct skip_list_node_t *prev = find_prev(msl, key, NULL);
    assert(prev != NULL);

    if (prev->next[0] != NULL && msl->keycmp(prev->next[0]->key, key) == 0) {
        return prev->next[0]->value;
    }

    return NULL;
}

const void *msl_erase(struct skip_list_t *msl, const void *key) {
    if (msl == NULL || key == NULL) {
        return NULL;
    }

    struct skip_list_node_t *prevs[MAX_HEIGHT];
    struct skip_list_node_t *prev = find_prev(msl, key, prevs);
    if (prev->next[0] == NULL || msl->keycmp(prev->next[0]->key, key) != 0) {
        return NULL;
    }

    struct skip_list_node_t *cur = prev->next[0];
    const void *old_value = cur->value;

    int level;
    for (level = 0; level <= msl->level; level++) {
        if (prevs[level]->next[level] != cur) {
            break;
        }
        prevs[level]->next[level] = cur->next[level];
    }

    free(cur);

    while (msl->level > 0 && msl->head.next[msl->level] == NULL) {
        msl->level--;
    }

    return old_value;
}

static const struct skip_list_iter_t null_iter;

struct skip_list_iter_t msl_iter_next(struct skip_list_t *msl, struct skip_list_iter_t *it) {
    if (msl == NULL || msl->head.next[0] == NULL) {
        return null_iter;
    }

    struct skip_list_iter_t next;
    next.v = msl->head.next[0];
    if (it != NULL) {
        next.v = it->v->next[0];
    }

    return next;
}

const void *msl_iter_getk(struct skip_list_iter_t *it) {
    if (it == NULL || it->v == NULL) {
        return NULL;
    }

    return it->v->key;
}

const void *msl_iter_getv(struct skip_list_iter_t *it) {
    if (it == NULL || it->v == NULL) {
        return NULL;
    }

    return it->v->value;
}

#ifdef SKIP_LIST_DEBUG

#include <stdio.h>

void msl_debug(struct skip_list_t *msl) {
    if (msl == NULL) {
        printf("skip_list is null\n");
        return;
    }

    int level;
    for (level = 0; level <= msl->level; level++) {
        printf("level(%d): ", level);
        struct skip_list_node_t *cur = msl->head.next[level];
        while (cur != NULL) {
            printf(" -> %s", (const char *)cur->key);
            cur = cur->next[level];
        }
        printf("\n");
    }
}

#endif














