#pragma once

#include <vector>

namespace GameEngine {

    class IOManager
    {
    public:
        static bool readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer);
    };

}