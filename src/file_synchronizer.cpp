#include "..//inc/file_synchronizer.hpp"

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
    std::vector<fs::path> configFiles, mainDirFiles, deletedFiles;
    fs::path configDirectoryPath = fs::current_path().parent_path() / "configDirectory";

    for (auto el : removedFiles) {}


    for (auto file : fs::recursive_directory_iterator(m_mainDirectoryPath)){
        if (fs::is_regular_file(file)){
            mainDirFiles.push_back(file.path());
        }

    }

    for (auto file : fs::recursive_directory_iterator(configDirectoryPath)){
        if (fs::is_regular_file(file)){
            configFiles.push_back(file.path());
        }

    }

     std::set_difference(configFiles.begin(), configFiles.end(),
                        mainDirFiles.begin(), mainDirFiles.end(),
                        std::back_inserter(deletedFiles),
                        [](auto& lhs, auto& rhs) { return lhs < rhs; });

    for (auto el : deletedFiles)
    {
        for (auto dir : fs::directory_iterator(m_mainDirectoryPath))
            try
            {
                if (!(fs::exists(dir / el.filename())))
                {
                    fs::copy_file(el , dir / el.filename(), fs::copy_options::update_existing);
                }
            }
            catch (std::exception &e)
            {
                std::cout << e.what();
            }
    }


/*
    for (auto el : deletedFiles)
    {
        for (auto dir : fs::directory_iterator(m_mainDirectoryPath))
            try
            {
                if (!(fs::exists(dir / el.filename())))
                {
                    fs::copy_file(el , dir / el.filename(), fs::copy_options::update_existing);
                }
            }
            catch (std::exception &e)
            {
                std::cout << e.what();
            }
    }




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
    */
}
/*
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
*/