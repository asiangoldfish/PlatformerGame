#pragma once

#include <filesystem>

class Image {
public:
    Image() = default;
    Image(std::string filepath);
    virtual ~Image();

private:
    unsigned char* data;

private: // Meta data
    std::string filepath;
    int width, height, nrChannels;
};