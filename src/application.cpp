#include "Application.hpp"
#include <windows.h>
#include <shlobj.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

namespace Birb
{
    void Application::Quit()
    {
        exit(0);
    }

    void Application::OpenURL(std::string url)
    {
        system(std::string("start " + url).c_str());
    }

    std::string Application::persistentDataPath()
    {
        std::string path;

        #ifdef __windows__
        if(SUCCEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, path)))
        {
            path.append("/LocalLow/Birb2D");
        }
        #elif __linux__
        const char *homedir;
            if((homedir = getenv("HOME")) == NULL)
            {
                homedir = getpwuid(getuid()) -> pw_dir;
            }
            path = homedir;
            path.append("/.config/Birb2D");
        #endif

        return path;
    }
}