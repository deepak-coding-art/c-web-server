#include <assert.h>
#include <stdlib.h>
#include "utils/utils.h"

void should_return_one_on_match()
{
    Request request = {
        "GET",
        "/index.html"};
    assert(match_method(&request, "GET") == 1);
}

void should_return_zero_on_not_match()
{
    Request request = {
        "GET",
        "/index.html"};
    assert(match_method(&request, "POST") == 0);
}

void should_return_zero_on_not_init_request()
{
    Request request;
    assert(match_method(&request, "GET") == 0);
}

void should_return_zero_on_not_init_and_null_request()
{
    Request request;
    assert(match_method(&request, NULL) == 0);
}

int main()
{
    should_return_one_on_match();
    should_return_zero_on_not_match();
    // should_return_zero_on_not_init_request();
    should_return_zero_on_not_init_and_null_request();
    return 0;
}