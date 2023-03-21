#include "..//inc/controller.hpp"
#include "..//inc/logger.hpp"
#include "..//inc/model.hpp"
#include "..//inc/thread_pool_provider.hpp"
#include "..//inc/view.hpp"
#include "..//inc/view_ftx.hpp"

Controller::Controller()
    : Controller(std::make_unique<ViewFTXuserInterface>(), std::make_unique<Model>())
{
}

Controller::Controller(std::unique_ptr<View> view, std::unique_ptr<Model> model) noexcept
    : m_view(std::move(view)), m_model(std::move(model))
{
    this->init();
}

void Controller::run()
{
    // if(typeid(m_view) == typeid(ViewFTXuserInterface))
    m_view->run(m_model->getMainDirectoryPath());
    m_view->setModel(m_model.get());
    /*
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
            std::invoke(h.m_handlerMap.at(mappedKey), this);
        }
    }
    */
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
        if (ErrorCode::SUCCESS != std::invoke(fun, this->m_model, input.value()))
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

    this->process(&Model::addDirectory);
}

void Controller::removeDirectory()
{
    m_view->printMessage(View::Message::Remove);

    this->process(&Model::removeDirectory);
}
void Controller::removeFile()
{
    m_view->printMessage(View::Message::RemoveFile);

    this->process(&Model::removeFile);
}
void Controller::printDirectory()
{
    m_view->printDirectory(m_model->getMainDirectoryPath());
    this->waitForButton();
}
void Controller::printFiles()
{
    std::set<fs::path> fileList;

    m_view->printMessage(View::Message::PrintFiles);

    if (auto input = getKeyboardInput(); input.has_value())
    {
        if (input.value() == "all" && ErrorCode::SUCCESS != m_model->getAllFilesInDir(m_model->getMainDirectoryPath(), fileList))
        {
            m_view->printMessage(View::Message::FolderEmpty);
            waitForButton();
        }

        if (input.value() != "all" && ErrorCode::SUCCESS != m_model->getAllFilesInDir(m_model->getMainDirectoryPath() / input.value(), fileList))
        {
            m_view->printMessage(View::Message::FolderEmpty);
            waitForButton();
        }
    }
    else
    {
        m_view->printMessage(View::Message::Incorrect);
        std::this_thread::sleep_for(Config_UISleepFor);
    }

    m_view->printFiles(fileList);
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

void Controller::exit()
{
    m_model->stopSync();

    m_isExitRequested = true;
}