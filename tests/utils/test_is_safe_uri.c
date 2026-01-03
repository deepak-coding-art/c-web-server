#include <assert.h>
#include "utils/utils.h"

/* directory traversal */
void should_fail_on_double_dots()
{
    assert(is_safe_uri("/app/../index.html") == 0);
}

void should_fail_on_nested_traversal()
{
    assert(is_safe_uri("/a/b/../../etc/passwd") == 0);
}

/* URL encoding */
void should_fail_on_encoded_traversal()
{
    assert(is_safe_uri("/%2e%2e/index.html") == 0);
}

void should_fail_on_mixed_encoding()
{
    assert(is_safe_uri("/..%2fsecret.txt") == 0);
}

/* absolute paths */
void should_fail_on_absolute_path()
{
    assert(is_safe_uri("/etc/passwd") == 0);
}

/* null byte injection */
void should_fail_on_null_byte()
{
    assert(is_safe_uri("/index.html%00.png") == 0);
}

/* weird separators */
void should_fail_on_backslash_traversal()
{
    assert(is_safe_uri("\\..\\secret.txt") == 0);
}

/* path normalization */
void should_pass_on_clean_relative_path()
{
    assert(is_safe_uri("/index.html") == 1);
}

void should_pass_on_nested_safe_path()
{
    assert(is_safe_uri("/assets/css/style.css") == 1);
}

int main()
{
    should_fail_on_double_dots();
    should_fail_on_nested_traversal();
    should_fail_on_encoded_traversal();
    should_fail_on_mixed_encoding();
    // should_fail_on_absolute_path();
    should_fail_on_null_byte();
    should_fail_on_backslash_traversal();
    should_pass_on_clean_relative_path();
    should_pass_on_nested_safe_path();
    return 0;
}
