#pragma once

#include <string>

namespace Birb
{
    enum NetworkReachability
	{
		NotReachable, WiFiNetwork, DataNetwork
	};

    class Application
    {
        public:
        static Quit();
        static OpenURL(std::string url);

        std::string persistentDataPath;

        bool isHeadless;
        bool isFocused;
    };

}