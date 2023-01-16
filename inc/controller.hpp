#pragma once

#include "model.hpp"
#include "view.hpp"
#include "i_ViewListener.hpp"

class Controller : public i_ViewListener
{
public:
    Controller(View *view, Model *model);

    virtual void addDirectory(std::istream &std_input) override;
    virtual void removeDirectory() override;
    virtual void removeFile() override;
    virtual void printDirectory() override;
    virtual void printFiles() override;
    virtual void setIntervalTime(std::istream &std_input) override;
    virtual void startSync() override;
    virtual void stopSync() override;
    virtual void forceSync() override;
    virtual void readConfig() override;
    virtual void saveConfig() override;
    virtual bool exit() override;

private:
    View *m_view;
    Model *m_model;
};