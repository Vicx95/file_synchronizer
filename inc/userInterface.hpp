#include <string>
#include <filesystem>

class UserInterface {
    const decltype(std::filesystem::current_path()) mainDirectoryPath = std::filesystem::current_path() / "../mainDirectory";
public:
    enum class Action { AddDir = 1,
                        RemoveDir = 2,
                        RemoveFile = 3,
                        PrintDir = 4,
                        PrintFiles = 5,
                        SetIntervalTime = 6,
                        ForceSync = 7,
                        Exit = 0 };
    UserInterface();
    void startSynchronizer();
    void printMenu();
    void printOptions();
    void makeAction(Action choice);

    void waitForButton();
    bool validationForRemoving(std::string name);

    void addDirectory();
    void removeDirectory();
    void removeFile();
    void printDirectory();
    void printFiles();
};