#include "Asset.h"
#include "FW_random.h"

Asset::Asset() {
    id = FW::generateUUID();
    if (name.empty()) {
        name = id;
    }
}
