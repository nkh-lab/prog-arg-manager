#include "ProgArgManager.h"

#include <iostream>
#include <vector>

using namespace nlab;

int main(int argc, char* argv[])
{
    // Optional numeric option with default value
    Option<int> num{"num", "to test numeric arguments", 5555, true};

    // Boolean options, always treated like optional, therefore no need to specify their optionality
    Option<bool> read{"read,r", "read flag to test boolean"};
    Option<bool> write{"write,w", "write flag to test boolean"};
    Option<bool> del{"delete,d", "delete flag to test boolean"};

    // Other types are mandatory options, if optionality is not set
    Option<std::string> path{"path,p", "path to test string"};
    Option<std::vector<std::string>> files{"files,f", "files to test vector<string>"};

    // ProgArgManager pam(argc, argv); // without version support
    ProgArgManager pam(argc, argv, "1.0.0");

    if (pam.parse(num, read, write, del, path, files))
    {
        // Here is the user's entry point where he can handle parced arguments and insert business logic.
        // Parce returns true when no generic (help and version) options were provided
        // and all mandatory were passed.

        std::cout << "num:   " << num.value << "\n";
        std::cout << "read:  " << read.value << "\n";
        std::cout << "write: " << write.value << "\n";
        std::cout << "del:   " << del.value << "\n";
        std::cout << "path:  " << path.value << "\n";

        std::cout << "files:\n";
        for (auto& f : files.value)
        {
            std::cout << "       " << f << "\n";
        }
    }

    return 0;
}
