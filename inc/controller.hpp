#pragma once

#include "model.hpp"
#include "view.hpp"

class Controller : public i_ViewListener
{
public:
    Controller(View *view, Model *model);

    virtual void addDirectory(std::istream &std_input) override;
    virtual void removeDirectory() override;
    virtual void removeFile() override;
    virtual void printDirectory() override;
    virtual void printFiles() override;
    virtual void setIntervalTime() override;
    virtual void startSync() override;
    virtual void forceSync() override;
    virtual bool exit() override;

private:
    View *m_view;
    Model *m_model;

    bool m_isExitRequested = true;
};