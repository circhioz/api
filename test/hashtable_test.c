#include <stdio.h>
#include "CuTest.h"
#include "hashtable.h"

void TestHashtable_Create(CuTest *tc) {
    hashtable_t* t = hashtable_create();
    CuAssertIntEquals(tc, 0, t->size);
    hashtable_destroy(t);
}

void TestHashtable_Set(CuTest *tc) {
    hashtable_t* t = hashtable_create();
    int res = hashtable_set(t, "asdf", NULL);
    CuAssertIntEquals(tc, 1, res);
    CuAssertIntEquals(tc, 1, t->size);
    res = hashtable_set(t, "asdf", NULL);
    CuAssertIntEquals(tc, 0, res);
    CuAssertIntEquals(tc, 1, t->size);
    hashtable_destroy(t);
}

void TestHashtable_Get(CuTest *tc) {
    hashtable_t* t = hashtable_create();
    void *a = (void *)500;
    hashtable_set(t, "asdf", a);
    void *res = hashtable_get(t, "asdf");
    CuAssertPtrEquals(tc, a, res);
    hashtable_destroy(t);
}

void TestHashtable_Remove(CuTest *tc) {
    hashtable_t* t = hashtable_create();
    hashtable_set(t, "asdf", NULL);
    hashtable_remove(t, "asdf");
    CuAssertIntEquals(tc, 0, t->size);
    void *res = hashtable_get(t, "asdf");
    CuAssertPtrEquals(tc, NULL, res);
    hashtable_destroy(t);
}

void TestHashtable_HammerTest(CuTest *tc) {
    char buffer[5];
    void *a = (void *)501;
    hashtable_t *t = hashtable_create();
    for(int i = 1; i <= 1024; i++) {
        sprintf(buffer, "%d", i);
        int res = hashtable_set(t, buffer, a);
        CuAssertIntEquals(tc, 1, res);
        CuAssertIntEquals(tc, i, t->size);
    }
    for(int i = 1; i <= 1024; i++) {
        sprintf(buffer, "%d", i);
        hashtable_remove(t, buffer);
        CuAssertIntEquals(tc, 1024 - i, t->size);
    }
    for(int i = 1; i <= 1024; i++) {
        sprintf(buffer, "%d", i);
        void *res = hashtable_get(t, buffer);
        CuAssertPtrEquals(tc, NULL, res);
    }
    hashtable_destroy(t);
}

CuSuite* CuHashtableGetSuite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestHashtable_Create);
    SUITE_ADD_TEST(suite, TestHashtable_Set);
    SUITE_ADD_TEST(suite, TestHashtable_Get);
    SUITE_ADD_TEST(suite, TestHashtable_Remove);
    SUITE_ADD_TEST(suite, TestHashtable_HammerTest);
    return suite;
}