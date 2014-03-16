#ifndef _SKIP_LIST_H
#define _SKIP_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

struct skip_list_t;
struct skip_list_node_t;

struct skip_list_iter_t {
    struct skip_list_node_t *v;
};

// key的比较函数
//  0: 相等
// -1: a < b
//  1: a > b
typedef int (*msl_keycmp_t)(const void *a, const void *b);

struct skip_list_t *msl_ini(msl_keycmp_t keycmp);
void msl_fini(struct skip_list_t *msl);

// key/value的生存期都必须由调用者保证
// 内部不做什么确认
// 0 : 成功
// -1: 失败
int msl_set(struct skip_list_t *msl, const void *key, const void *value);
const void *msl_get(struct skip_list_t *msl, const void *key);
// 返回旧值(不存在即NULL)
const void *msl_erase(struct skip_list_t *msl, const void *key);

/*
 *
 * msl_iter_next(msl, NULL)返回开始
 * v == NULL表示结束
 *
 */
struct skip_list_iter_t msl_iter_next(struct skip_list_t *msl, struct skip_list_iter_t *it);
const void *msl_iter_getk(struct skip_list_iter_t *it);
const void *msl_iter_getv(struct skip_list_iter_t *it);

#ifdef SKIP_LIST_DEBUG
void msl_debug(struct skip_list_t *msl);
#else
#define msl_debug(x) ((void)(x))
#endif

#ifdef __cplusplus
}
#endif

#endif
