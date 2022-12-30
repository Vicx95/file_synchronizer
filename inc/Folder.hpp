#pragma once
#include <memory>
#include <vector>
#include "File.hpp"


class Folder {
public:
private:
    std::vector<std::unique_ptr<File>> m_files;
};