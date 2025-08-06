#include "Image.h"
#include <stb_image.h>
#include "assertions.h"

#include "Framework.h"

Image::Image(std::string filepath) {
    this->filepath = filepath;
    data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    ASSERT(data, "stb_image failed to load file \'" + filepath + "\'");
}

Image::~Image() {
    if (data) {
        stbi_image_free(data);
    }
}
