#include "..//inc/controller.hpp"
#include "..//inc/logger.hpp"
#include "..//inc/thread_pool_provider.hpp"

Controller::Controller()
{
    // TODO provide simple factory
    // Controller(std::move(std::make_unique<View>()), std::move(std::make_unique<Model>()));
}

Controller::Controller(std::unique_ptr<View> view, Model *model)
    : m_view(std::move(view)), m_model(model)
{
    this->init();
}

void Controller::run()
{
    while (!m_isExitRequested)
    {
        m_view->printMenu();
        m_view->printOptions();

        if (auto input = getKeyboardInput(); !input.has_value())
        {
            std::this_thread::sleep_for(Config_UISleepFor);
            m_view->printMessage(View::Message::Incorrect);

            continue;
        }
        else if (Handlers::Action mappedKey = static_cast<Handlers::Action>(std::stoul(input.value())); h.m_handlerMap.contains(mappedKey))
        {
            h.m_handlerMap.at(mappedKey)(this);
        }
    }
}

void Controller::init()
{
    h.m_handlerMap = {
        {Handlers::Action::AddDir, &Controller::addDirectory}, //
        {Handlers::Action::RemoveDir, &Controller::removeDirectory},
        {Handlers::Action::RemoveFile, &Controller::removeFile},
        {Handlers::Action::PrintDir, &Controller::printDirectory},
        {Handlers::Action::PrintFiles, &Controller::printFiles},
        {Handlers::Action::SetIntervalTime, &Controller::setIntervalTime},
        {Handlers::Action::StartSync, &Controller::startSync},
        {Handlers::Action::StopSync, &Controller::stopSync},
        {Handlers::Action::ForceSync, &Controller::forceSync},
        {Handlers::Action::ReadConfig, &Controller::readConfig},
        {Handlers::Action::SaveConfig, &Controller::saveConfig},
        {Handlers::Action::Exit, &Controller::exit} //
    };
}

void Controller::waitForButton()
{
    std::system("/bin/bash -c \"read -n 1 -s -p \"PressAnyKeyToContinue...\"\"");
}

std::optional<std::string> Controller::getKeyboardInput(std::regex keyRegex)
{
    // TODO: prepare regexes
    std::string input;

    std::cin.clear();
    std::cin >> input;

    if (!std::regex_search(input, keyRegex))
    {
        return {};
    }

    return input;
}

void Controller::process(auto fun)
{
    if (auto input = getKeyboardInput(); input.has_value())
    {
        if (ErrorCode::SUCCESS != fun(input.value()))
        {
            m_view->printMessage(View::Message::DirExists);
            waitForButton();
        }
    }
    else
    {
        m_view->printMessage(View::Message::Incorrect);
        std::this_thread::sleep_for(Config_UISleepFor);
    }
}

void Controller::addDirectory()
{
    m_model->createMainDir();
    m_view->printMessage(View::Message::GiveFolder);

    this->process(std::forward<decltype(Model::addDirectory)>(Model::addDirectory));
}

void Controller::removeDirectory()
{
    m_view->printMessage(View::Message::Remove);

    this->process(std::forward<decltype(Model::removeDirectory)>(Model::removeDirectory));
}
void Controller::removeFile()
{
    m_view->printMessage(View::Message::RemoveFile);

    this->process(std::forward<decltype(Model::removeFile)>(Model::removeFile));
}
void Controller::printDirectory()
{
    m_view->printDirectory(m_model->getMainDirectoryPath());
    this->waitForButton();
}
void Controller::printFiles()
{
    // TODO finish refactor
    /*     std::set<fs::path> sorted_by_name;
        std::cout << "Give folder name or choose 'all' to print files: \n";
        std::string dirName;
        std::cin.clear();
        std::cin >> dirName;

        if (validateForPrinting(dirName))
        {
            if (dirName == "all")
            {
                dirName = mainDirectoryPath;
            }
            for (auto const &dirEntry : fs::recursive_directory_iterator(dirName))
            {
                sorted_by_name.insert(dirEntry.path());
            }

    if (fs::is_empty(mainDirectoryPath / dirName))
        std::cout << "Folder is empty...";

    std::cout << "\n";*/
    // m_view->printFiles();
    this->waitForButton();
}
void Controller::setIntervalTime()
{
    m_view->printMessage(View::Message::Interval);

    if (auto input = getKeyboardInput(); input.has_value())
    {
        m_model->setIntervalTime(input.value());
    }
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