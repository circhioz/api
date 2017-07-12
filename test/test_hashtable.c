#include "cheat.h"
#include "cheats.h"
#include "utils.h"
#include "hashtable.h"

CHEAT_DECLARE(
    hashtable_t *t;
    void *a = (void *) 501;
)

CHEAT_SET_UP(
    t = hashtable_create();
)

CHEAT_TEAR_DOWN(
    hashtable_destroy(t);
)

CHEAT_TEST(test_hashtable_create,
    cheat_assert_size(t->size, 0);
)

CHEAT_TEST(test_hashtable_set,
    cheat_assert(hashtable_set(t, "asdf", NULL));
    cheat_assert_size(hashtable_get_size(t), 1);
    cheat_assert_not(hashtable_set(t, "asdf", NULL));
    cheat_assert_size(hashtable_get_size(t), 1);
)

CHEAT_TEST(test_hashtable_get,
    cheat_assert(hashtable_set(t, "asdf", a));
    cheat_assert_pointer(hashtable_get(t, "asdf"), a);
)

CHEAT_TEST(test_hashtable_remove,
    cheat_assert(hashtable_set(t, "asdf", a));
    hashtable_remove(t, "asdf");
    cheat_assert_size(hashtable_get_size(t), 0);
    cheat_assert_pointer(hashtable_get(t, "asdf"), NULL);
)

CHEAT_TEST(test_hashtable_hammer,
    char *keys[1024];
    for (size_t i = 1; i <= 1024; i++) {
        keys[i - 1] = malloc_or_die(5 * sizeof(char));
        sprintf(keys[i - 1], "%d", (int) i);
        cheat_assert(hashtable_set(t, keys[i - 1], a));
        cheat_assert_size(hashtable_get_size(t), i);
    }
    for (size_t i = 1; i <= 1024; i++) {
        hashtable_remove(t, keys[i - 1]);
        cheat_assert_size(hashtable_get_size(t), 1024 - i);
    }
    for (size_t i = 1; i <= 1024; i++) {
        cheat_assert_pointer(hashtable_get(t, keys[i - 1]), NULL);
        free(keys[i - 1]);
    }
)