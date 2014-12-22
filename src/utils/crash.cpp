#include "utils/crash.h"

#include <cstdio>
#include <signal.h>

#include "utils/utils.h"

void outputBacktrace(void *address, const char *signal)
{
    FILE *file = std::fopen("crash.txt", "w");

    std::fprintf(file, "Signal: %s\nAddress: %p\nBacktrace:\n", signal, address);

    std::vector<std::string> backtrace = getBacktrace();

    for (std::vector<std::string>::iterator it = backtrace.begin();
         it != backtrace.end(); ++it)
    {
        std::fprintf(file, "    %s\n", it->c_str());
    }

    std::fclose(file);
}

extern "C" void segFault(int signal, siginfo_t *info, void *context)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;

    sa.sa_handler = SIG_DFL;
    sigaction(SIGSEGV, &sa, NULL);

    outputBacktrace(info->si_addr, "Segmentation fault");
}

extern "C" void fpe(int signal, siginfo_t *info, void *context)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;

    sa.sa_handler = SIG_DFL;
    sigaction(SIGFPE, &sa, NULL);

    outputBacktrace(info->si_addr, "Floating point exception");
}

extern "C" void ill(int signal, siginfo_t *info, void *context) //TODO: Does this work?
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;

    sa.sa_handler = SIG_DFL;
    sigaction(SIGILL, &sa, NULL);

    outputBacktrace(info->si_addr, "Illegal instruction");
}

void initCrashSystem()
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;

    sa.sa_sigaction = segFault;
    sigaction(SIGSEGV, &sa, NULL);

    sa.sa_sigaction = fpe;
    sigaction(SIGFPE, &sa, NULL);

    sa.sa_sigaction = ill;
    sigaction(SIGILL, &sa, NULL);
}
