#include <filesystem>
#include <string>

enum class ErrorCode { SUCCESS = 0, FAIL = 1};

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
    bool validationForPrinting(std::string name);

    ErrorCode addDirectory(std::istream &std_input);
    ErrorCode removeDirectory();
    ErrorCode removeFile();
    void printDirectory();
    void printFiles();
};