#include "doctest/doctest.h"

#include "Files.h"

TEST_CASE("getDataDir() returns nonempty filepath") {
    unsetenv("XDG_DATA_DIR");
    unsetenv("HOME");
    // setenv("XDG_DATA_DIR", "/mock/data", 1);
    auto result = FW::getDataDir();
    CHECK(result);
    if (result) {
        CHECK(!result.value().string().empty());
    }
}