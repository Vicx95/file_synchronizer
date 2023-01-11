#include "..//inc/FileSynchronizer.hpp"

#include <iostream>

void FileSynchronizer::synchronizeAdded(const std::vector<PathTimePair_t> &addedFiles)
{
    for (auto el : addedFiles)
    {
        if (fs::is_regular_file(el.first))
        {
            for (auto dir : fs::directory_iterator(el.first.parent_path().parent_path()))
                try
                {
                    if (el.first != dir / el.first.filename())
                        fs::copy_file(el.first, dir / el.first.filename(), fs::copy_options::update_existing);
                }
                catch (std::exception &e)
                {
                    std::cout << e.what();
                }
        }
    }
}

void FileSynchronizer::synchronizeRemoved(const std::vector<PathTimePair_t> &removedFiles)
{
    for (auto el : removedFiles)
    {
        for (auto dir : fs::directory_iterator(el.first.parent_path().parent_path()))
            try
            {
                if (el.first != dir / el.first.filename() && !(fs::exists(el.first)))
                {
                    fs::remove(dir / el.first.filename());
                }
            }
            catch (std::exception &e)
            {
                std::cout << e.what();
            }
    }
}