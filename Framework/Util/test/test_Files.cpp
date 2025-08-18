#include "doctest/doctest.h"

#include "Files.h"
#include "FW_random.h"

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

TEST_CASE("uuid is 128 bits") {
    std::string result = FW::generateUUID();
    REQUIRE(!result.empty());

    // The UUID has hyphens, so it's 36 bytes instead of 32.
    CHECK((result.size() == 36));
}
