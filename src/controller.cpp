#include "..//inc/controller.hpp"

Controller::Controller(View *view, Model *model)
    : m_view(view), m_model(model)
{
    m_view->setMainDirectoryPath(m_model->getMainDirectoryPath());

}

void Controller::addDirectory(std::istream &std_input)
{
    m_model->addDirectory(std_input);
}
void Controller::removeDirectory(std::istream &std_input)
{
    m_model->removeDirectory(std_input);
}
void Controller::removeFile()
{
    m_model->removeFile();
}
std::vector<std::vector<std::string>> Controller::printDirectory()
{
    return m_view->printDirectory();
}
std::vector<std::vector<std::string>> Controller::printFiles()
{
    return m_view->printFiles();
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
    m_model->stopSync();
    return true;
}