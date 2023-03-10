#include "..//inc/controller.hpp"
#include "..//inc/logger.hpp"
#include "../inc/thread_pool_provider.hpp"

Controller::Controller(View *view, Model *model)
    : m_view(view), m_model(model)
{
    this->init();
}

void Controller::run()
{

    std::string inputKey;
    std::regex keyRegex("([0-9]{1})");

    while (!m_isExitRequested)
    {
        m_view->printMenu();
        m_view->printOptions();

        std::cin.clear();
        std::cin >> inputKey;

        if (!std::regex_search(inputKey, keyRegex))
        {
            std::cout << "Incorrect action selected! Please try again...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            continue;
        }

        if (Handlers::Action mappedKey = static_cast<Handlers::Action>(std::stoul(inputKey)); h.m_handlerMap.contains(mappedKey))
        {
            h.m_handlerMap.at(mappedKey)();
        }
        /*
                switch (static_cast<Handlers::Action>(std::stoul(inputKey)))
                {
                case Handlers::Action::AddDir:
                    this->addDirectory(std::cin);
                    break;
                case Action::RemoveDir:
                    this->removeDirectory();
                    break;
                case Action::RemoveFile:
                    this->removeFile();
                    break;
                case Action::PrintDir:
                    this->printDirectory();
                    break;
                case Action::PrintFiles:
                    this->printFiles();
                    break;
                case Action::SetIntervalTime:
                    this->setIntervalTime(std::cin);
                    break;
                case Action::StartSync:
                    this->startSync();
                    break;
                case Action::StopSync:
                    this->stopSync();
                    break;
                case Action::ForceSync:
                    this->forceSync();
                    break;
                case Action::readConfig:
                    this->readConfig();
                    break;
                case Action::saveConfig:
                    this->saveConfig();
                    break;
                case Action::Exit:
                    m_isExitRequested = this->exit();
                    break;
                default:
                    std::cout << "Incorrect action selected! Please try again...\n";
                    waitForButton();
                    break;
                }*/
    }
}

void Controller::init()
{
    h.m_handlerMap = {{Handlers::Action::AddDir, [this]() { this->removeDirectory(); }}};
}

void Controller::waitForButton()
{
    std::system("/bin/bash -c \"read -n 1 -s -p \"PressAnyKeyToContinue...\"\"");
}

void Controller::addDirectory(std::istream &std_input)
{
    m_model->addDirectory(std_input);
}
void Controller::removeDirectory()
{
    m_model->removeDirectory();
}
void Controller::removeFile()
{
    m_model->removeFile();
}
void Controller::printDirectory()
{
    m_view->printDirectory(m_model->getMainDirectoryPath());
    this->waitForButton();
}
void Controller::printFiles()
{
    // m_view->printFiles();
    this->waitForButton();
}
void Controller::setIntervalTime(std::istream &std_input)
{
    m_model->setIntervalTime(std_input);
}
void Controller::startSync()
{
    m_model->startSync();
}
void Controller::stopSync()
{
    m_model->stopSync();
}
void Controller::forceSync()
{
    m_model->forceSync();
}

void Controller::readConfig()
{
    m_model->readConfig();
}
void Controller::saveConfig()
{
    m_model->saveConfig();
}

bool Controller::exit()
{
    LOG_INFO("Stopping synchronizer");
    m_model->stopSync();
    return true;
}