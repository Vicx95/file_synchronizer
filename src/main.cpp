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
        Model m(std::make_unique<Timer>(), std::make_unique<FileSynchronizer>(), std::make_unique<Scanner>());
        Controller c(std::make_unique<View>(), &m);
        c.run();
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}