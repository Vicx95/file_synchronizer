#include "..//inc/view.hpp"
#include "..//inc/view_console.hpp"

void View::printMenu()
{
    std::cout << "\n\n ### FILE SYNCHRONIZER ### \n\n";
}

void View::printOptions()
{
    std::cout << "0. Exit \n-------------------------\n"
              << "1. Add new directory \n"
              << "2. Remove directory \n"
              << "3. Remove file \n"
              << "4. Print all directories \n"
              << "5. Print all files \n"
              << "6. Set interval time  \n"
              << "7. Start sync-up  \n"
              << "8. Stop sync-up  \n"
              << "9. Force sync-up  \n"
              << "10. Read config  \n"
              << "11. Save config  \n";
}

void View::printDirectory(const fs::path &path)
{
    fs::current_path(path);
    std::cout << "Print current path: " << path << "\n\n";

    for (auto const &dirEntry : fs::directory_iterator(path))
    {
        std::cout << dirEntry.path() << std::endl;
    }
    if (fs::is_empty(path))
        std::cout << "Folder is empty...\n";
}

bool View::validateForPrinting(std::string name)
{
    if (name == "all")
    {
        return true;
    }
    if (!fs::exists(fs::current_path() / name))
    {
        std::cout << "File or directory doesn't exist\n";
        return false;
    }
    return true;
}

void View::printMessage(Message m)
{
    switch (m)
    {
    case Message::GiveFolder:
        std::cout << "Give folder name to add: \n";
        break;
    case Message::DirExists:
        std::cout << "Dir already exist...\n";
        break;
    case Message::Incorrect:
        std::cout << "Incorrect action selected! Please try again...\n";
        break;
    case Message::Interval:
        std::cout << "Interval time value [milliseconds]: \n";
        break;
    case Message::Remove:
        std::cout << "Give folder name to remove: \n";
        break;
    case Message::RemoveFile:
        std::cout << "Give folder name with files to delete: \n";
        break;
    case Message::PrintFiles:
        std::cout << "Give folder name or choose 'all' to print files: \n";
        break;
    case Message::FolderEmpty:
        std::cout << "Folder is empty...";
        break;
    default:
        std::cout << "Message string not found";
        break;
    }
}

void View::printFiles(const std::set<fs::path> &fileDirList)
{
    for (auto const &fileName : fileDirList)
    {
        if (fs::is_directory(fileName))
        {
            std::cout << "-------------------\n";
            std::cout << "Directory: " << fileName.filename() << ":\n";
        }
        else if (fs::is_regular_file(fileName))
        {
            std::cout << fileName.filename() << "\n";
        }
    }
}
/*
void ViewConsoleUserInterface::run(const fs::path &path)
{
    fs::path spath = path;
    printMenu();
    printOptions();
}
*/ 
