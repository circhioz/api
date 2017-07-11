#include <stdio.h>
#include "CuTest.h"
#include "utils.h"
#include "hashtable.h"

void test_hashtable_create(CuTest *tc) {
    hashtable_t *t = hashtable_create();
    CuAssertIntEquals(tc, 0, t->size);
    hashtable_destroy(t);
}

void test_hashtable_set(CuTest *tc) {
    hashtable_t *t = hashtable_create();
    int res = hashtable_set(t, "asdf", NULL);
    CuAssertIntEquals(tc, 1, res);
    CuAssertIntEquals(tc, 1, t->size);
    res = hashtable_set(t, "asdf", NULL);
    CuAssertIntEquals(tc, 0, res);
    CuAssertIntEquals(tc, 1, t->size);
    hashtable_destroy(t);
}

void test_hashtable_get(CuTest *tc) {
    hashtable_t *t = hashtable_create();
    void *a = (void *) 500;
    hashtable_set(t, "asdf", a);
    void *res = hashtable_get(t, "asdf");
    CuAssertPtrEquals(tc, a, res);
    hashtable_destroy(t);
}

void test_hashtable_remove(CuTest *tc) {
    hashtable_t *t = hashtable_create();
    hashtable_set(t, "asdf", NULL);
    hashtable_remove(t, "asdf");
    CuAssertIntEquals(tc, 0, t->size);
    void *res = hashtable_get(t, "asdf");
    CuAssertPtrEquals(tc, NULL, res);
    hashtable_destroy(t);
}

void test_hashtable_hammer(CuTest *tc) {
    void *a = (void *) 501;
    hashtable_t *t = hashtable_create();
    char *keys[1024];
    for (int i = 1; i <= 1024; i++) {
        keys[i - 1] = malloc_or_die(5 * sizeof(char));
        sprintf(keys[i - 1], "%d", i);
        int res = hashtable_set(t, keys[i - 1], a);
        CuAssertIntEquals(tc, 1, res);
        CuAssertIntEquals(tc, i, t->size);
    }
    for (int i = 1; i <= 1024; i++) {
        hashtable_remove(t, keys[i - 1]);
        CuAssertIntEquals(tc, 1024 - i, t->size);
    }
    for (int i = 1; i <= 1024; i++) {
        void *res = hashtable_get(t, keys[i - 1]);
        CuAssertPtrEquals(tc, NULL, res);
        free(keys[i - 1]);
    }
    hashtable_destroy(t);
}

CuSuite *CuHashtableGetSuite() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_hashtable_create);
    SUITE_ADD_TEST(suite, test_hashtable_set);
    SUITE_ADD_TEST(suite, test_hashtable_get);
    SUITE_ADD_TEST(suite, test_hashtable_remove);
    SUITE_ADD_TEST(suite, test_hashtable_hammer);
    return suite;
}