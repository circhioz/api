#include "cheat.h"
#include "cheats.h"
#include "simplefs.h"

CHEAT_DECLARE(
    node_t *root;
)

CHEAT_SET_UP(
    root = fs_new_root();
)

CHEAT_TEAR_DOWN(
    fs_destroy_root(root);
)

CHEAT_TEST(test_fs_new_root,
    cheat_assert_not_pointer(root, NULL);
)

CHEAT_TEST(test_fs_get_type,
    cheat_assert_uint8(fs_get_type(root), Dir);
)

CHEAT_TEST(test_fs_find_in_dir__ok,
    fs_create(root, "file1", File);
    node_t *file = fs_find_in_dir(root, "file1");
    cheat_assert_not_pointer(file, NULL);
    cheat_yield();
    fs_delete(file, false);
)

CHEAT_TEST(test_fs_find_in_dir__fail,
    cheat_assert_pointer(fs_find_in_dir(root, "file1"), NULL);
)

CHEAT_TEST(test_fs_create__file_ok,
    // File creation
    cheat_assert(fs_create(root, "file1", File));
    node_t *file = fs_find_in_dir(root, "file1");
    cheat_assert_not_pointer(file, NULL);
    cheat_yield();
    cheat_assert_uint8(fs_get_type(file), File);
    cheat_assert_string(fs_get_file_content(file), "");
    fs_delete(file, false);
)

CHEAT_TEST(test_fs_create__dir_ok,
    // Directory creation
    cheat_assert(fs_create(root, "dir1", Dir));
    node_t *dir = fs_find_in_dir(root, "dir1");
    cheat_assert_not_pointer(dir, NULL);
    cheat_yield();
    cheat_assert_uint8(fs_get_type(dir), Dir);
    fs_delete(dir, false);
)

CHEAT_TEST(test_fs_create__fail_name_lenght,
    // Name is too long, should fail
    char *name = "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
    cheat_assert_not(fs_create(root, name, File));
    cheat_assert_pointer(fs_find_in_dir(root, name), NULL);
)

CHEAT_TEST(test_fs_create__fail_already_exist,
    // Resource with the given name already exists, fail
    fs_create(root, "file1", File);
    cheat_assert_not(fs_create(root, "file1", Dir));
    fs_delete(fs_find_in_dir(root, "file1"), false);
)

CHEAT_TEST(test_fs_create__fail_max_depth,
    node_t *node = root;
    for(int i = 0; i < MAX_DEPTH + 1; i++) {
        if(i < MAX_DEPTH) {
            cheat_assert(fs_create(node, "dir1", Dir));
            node = fs_find_in_dir(node, "dir1");
        } else {
            cheat_assert_not(fs_create(node, "dir1", Dir));
        }
    }
    fs_delete(fs_find_in_dir(root, "dir1"), true);
)

CHEAT_TEST(test_fs_create__fail_max_nodes,
    fs_create(root, "dir1", Dir);
    node_t *node = fs_find_in_dir(root, "dir1");
    char buffer[5];
    for(int i = 0; i < MAX_NODES + 1; i++) {
        sprintf(buffer, "%d", i);
        if(i < MAX_NODES) {
            cheat_assert(fs_create(node, buffer, Dir));
        } else {
            cheat_assert_not(fs_create(node, buffer, Dir));
        }
    }
    fs_delete(node, true);
)

CHEAT_TEST(test_fs_get_path,
     fs_create(root, "dir1", Dir);
     node_t *node = fs_find_in_dir(root, "dir1");
     char *path = fs_get_path(node, 0);
     cheat_assert_string(path, "/dir1");
     free(path);
     path = fs_get_path(root, 0);
     cheat_assert_string(path, "/");
     free(path);
     fs_delete(node, true);
)

CHEAT_TEST(test_fs_set$get_file_content__ok,
     fs_create(root, "file1", File);
     node_t *node = fs_find_in_dir(root, "file1");
     cheat_assert(fs_set_file_content(node, "Lorem ipsum"));
     cheat_assert_string(fs_get_file_content(node), "Lorem ipsum");
     fs_delete(node, false);
)

CHEAT_TEST(test_fs_set_file_content__dir_fail,
     cheat_assert_not(fs_set_file_content(root, "Lorem ipsum"));
)

CHEAT_TEST(test_fs_get_file_content__dir_fail,
     cheat_assert_pointer(fs_get_file_content(root), NULL);
)

CHEAT_TEST(test_fs_delete__file_ok,
     fs_create(root, "file1", File);
     node_t *node = fs_find_in_dir(root, "file1");
     cheat_assert(fs_delete(node, false));
     cheat_assert_pointer(fs_find_in_dir(root, "file1"), NULL);
)

CHEAT_TEST(test_fs_delete__empty_dir_ok,
     fs_create(root, "dir1", Dir);
     node_t *node = fs_find_in_dir(root, "dir1");
     cheat_assert(fs_delete(node, false));
     cheat_assert_pointer(fs_find_in_dir(root, "dir1"), NULL);
)

CHEAT_TEST(test_fs_delete__nonempty_dir_ok,
     fs_create(root, "dir1", Dir);
     node_t *dir1 = fs_find_in_dir(root, "dir1");
     fs_create(dir1, "dir2", Dir);
     fs_create(dir1, "file1", File);
     cheat_assert(fs_delete(dir1, true));
     cheat_assert_pointer(fs_find_in_dir(root, "dir1"), NULL);
)

CHEAT_TEST(test_fs_delete__nonempty_dir_fail,
     fs_create(root, "dir1", Dir);
     node_t *dir1 = fs_find_in_dir(root, "dir1");
     fs_create(dir1, "dir2", Dir);
     node_t *dir2 = fs_find_in_dir(dir1, "dir2");
     cheat_assert_not(fs_delete(dir1, false));
     fs_delete(dir2, false);
     fs_delete(dir1, false);
)

CHEAT_TEST(test_fs_find_r,
     fs_create(root, "dir1", Dir);
     fs_create(root, "file1", File);
     node_t *file1 = fs_find_in_dir(root, "file1");
     node_t *dir1 = fs_find_in_dir(root, "dir1");
     fs_create(dir1, "dir2", Dir);
     fs_create(dir1, "file1", File);
     fs_create(dir1, "file2", File);
     node_t *file11 = fs_find_in_dir(dir1, "file1");
     size_t nres = 0;
     node_t **res = fs_find_r(root, "file1", &nres, NULL);
     cheat_assert_size(nres, 2);
     if(res[0] == file1) {
         cheat_assert_pointer(res[1], file11);
     } else {
         cheat_assert_pointer(res[0], file11);
         cheat_assert_pointer(res[1], file1);
     }
     free(res);
     fs_delete(file1, true);
     fs_delete(dir1, true);
)