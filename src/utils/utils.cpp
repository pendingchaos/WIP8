#include "utils/utils.h"

#include <execinfo.h>
#include <cxxabi.h>
#include <dlfcn.h>
#include <cstdlib>

std::vector<std::string> getBacktrace()
{
    std::vector<std::string> res;

    void *trace[256];

    unsigned int depth = backtrace(trace, 256);

    for (unsigned int i=0; i<depth; ++i)
    {
        Dl_info info;

        if (dladdr(trace[i] , &info) == 0)
        {
            continue;
        }

        const char *name = info.dli_sname;

        int status;

        char *demangled = abi::__cxa_demangle(name, NULL, 0, &status);

        if (status == 0 and demangled != NULL)
        {
            res.push_back(demangled);
        } else if (name != NULL)
        {
            res.push_back(name);
        }

        if (status == 0)
        {
            std::free(demangled);
        }
    }

    return res;
}
