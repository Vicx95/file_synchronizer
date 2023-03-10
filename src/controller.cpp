#include "..//inc/controller.hpp"
#include "..//inc/logger.hpp"

Controller::Controller(View *view, Model *model)
    : m_view(view), m_model(model)
{
    m_view->setMainDirectoryPath(m_model->getMainDirectoryPath());
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
    m_view->printDirectory();
}
void Controller::printFiles()
{
    m_view->printFiles();
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