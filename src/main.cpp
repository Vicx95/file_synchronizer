#include "inc/controller.hpp"
#include "inc/file_synchronizer.hpp"
#include "inc/logger.hpp"
#include "inc/model.hpp"
#include "inc/scanner.hpp"
#include "inc/serializer.hpp"
#include "inc/timer.hpp"
#include "inc/view.hpp"
#include "inc/view_ftx.hpp"
#include "inc/view_console.hpp"

int main()
{
    try
    {
        LOG_INFO("Synchronizer starting");

        std::unique_ptr<Model> m = std::make_unique<Model>();
        std::unique_ptr<View> v = std::make_unique<ViewConsoleUserInterface>();
        //std::unique_ptr<View> v = std::make_unique<ViewFTXuserInterface>();
        Controller c(std::move(v), std::move(m));
        c.run();

        LOG_INFO("Synchronizer stopping");
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}