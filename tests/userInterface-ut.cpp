#include <gtest/gtest.h>
#include "..//inc/userInterface.hpp"

TEST(UserInterfaceTest, addingNewDirectoryShouldPass)
{  
    ErrorCode result = ErrorCode::FAIL;
    std::string input = "Dir99";
    std::filesystem::path expected = "/Users/pawellos/projects/file_synchronizer/build/../mainDirectory/Dir99";
    
    std::stringstream fakeInput(input);
    UserInterface userInterface;
    result = userInterface.addDirectory(fakeInput);

    if(!std::filesystem::exists(expected))
        result = ErrorCode::FAIL;
    ASSERT_EQ(result, ErrorCode::SUCCESS);
    std::filesystem::remove_all(expected);
}

TEST(UserInterfaceTest, addingExistingDirectoryShouldFail)
{  
    ErrorCode result = ErrorCode::FAIL;
    std::string input = "Dir1";
    std::filesystem::path expected = "/Users/pawellos/projects/file_synchronizer/build/../mainDirectory/Dir1";
    
    std::stringstream fakeInput(input);
    UserInterface userInterface;
    result = userInterface.addDirectory(fakeInput);

    ASSERT_EQ(result, ErrorCode::FAIL);
}
