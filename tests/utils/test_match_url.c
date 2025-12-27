#include <assert.h>
#include <stdlib.h>
#include "../../include/utils.h"

void should_return_one_on_match()
{
    Request request = {
        "GET",
        "/index"};
    assert(match_url(&request, "/index") == 1);
}

void should_return_zero_on_not_match()
{
    Request request = {
        "GET",
        "/about"};
    assert(match_url(&request, "index") == 0);
}

int main()
{
    should_return_one_on_match();
    should_return_zero_on_not_match();
    return 0;
}