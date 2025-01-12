#include "stdafx.h"
#include "filemapper.h"
#include "json_ver1.h"
#include "timemeasurer.h"

/// -------------------------------------------------------
int main(int argc, char* argv[])
{
    std::cout << "I am generating json demo project!" << std::endl;
    if (argc < 2){
        std::cout << "file to parse needed as parameter" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        TimeMeasurer fulltime("Processing took");

        FileMapper srcData(argv[1]);
        std::span<const unsigned char > src = srcData.filedata();

        std::string sTargetname{argv[1]};
        sTargetname.append(".json");

        return ver1::ProcessTheStream(src, sTargetname.c_str());
    }
#if !defined(__linux__) && !(defined(__APPLE__) && defined(__MACH__))
    // WINDOWS
    catch (const std::runtime_error& err) {
        // Handle the error by printing an error message
        std::cerr << "Runtime error: " << err.what() << std::endl;
        return EXIT_FAILURE;  // Return an error code from the main program
    }
#else
    catch (const std::system_error& err) {
        // Handle system errors specifically
        std::cerr << "System error: " << err.what() << "\n";
        std::cerr << "Error code: " << err.code() << " - " << err.code().message() << '\n';
        return EXIT_FAILURE;
    }
#endif

    catch (const std::exception& e) {
        // General catch block for any other types of exceptions
        std::cerr << "General exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        // Catch block for any other types of unknown exceptions
        std::cerr << "Unknown exception caught." << std::endl;
        return EXIT_FAILURE;
    }

//    return EXIT_SUCCESS;
}
