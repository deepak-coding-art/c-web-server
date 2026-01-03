#include <assert.h>
#include <utils/utils.h>

void should_extract_ext()
{
    char *ext = extract_file_ext("/web/index.html");
    assert(ext != NULL);
    assert((strcmp(ext, "html")) == 0);
}

void should_return_null()
{
    char *ext = extract_file_ext("/web/index");
    assert(ext == NULL);
}

int main()
{
    should_extract_ext();
    should_return_null();
    return 0;
}