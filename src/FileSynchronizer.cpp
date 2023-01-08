#include "..//inc/FileSynchronizer.hpp"

#include <iostream>

FileSynchronizer::FileSynchronizer(Scanner *scanner)
    : m_scanner(scanner)
{
}

FileSynchronizer::~FileSynchronizer()
{
    // m_timer.stop();
}

void FileSynchronizer::synchronize(const Path_t &path)
{
    this->m_scanner->scan(path);
    std::puts("Synchronizuje!");
}