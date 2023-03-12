#include "..//inc/controller.hpp"
#include "..//inc/file_synchronizer.hpp"
#include "..//inc/logger.hpp"
#include "..//inc/model.hpp"
#include "..//inc/scanner.hpp"
#include "..//inc/serializer.hpp"
#include "..//inc/timer.hpp"
#include "..//inc/view.hpp"

int main()
{
    LOG_INFO("Synchronizer starting");

    try
    {
        Controller c;
        c.run();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}