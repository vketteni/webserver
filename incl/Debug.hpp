#include <iostream>
#include <sstream>

#define COLOR  "\033[1;92m"
#define RESET "\033[0m"

#ifdef NDEBUG
#define debug(...)
#define header(...)
#define newLine()
#else
#define debug(msg) \
    std::cerr << COLOR << __FILE__ << ":" << __LINE__ << ": " << __func__ << ": " << msg << RESET << std::endl;
#define header(msg) \
    std::cerr << COLOR << "======= " << msg << " =======" << RESET << std::endl;
#define newLine() \
    std::cerr << std::endl;
#endif


// Farben für die Konsole (ANSI Escape Codes)
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[1;92m"
#define COLOR_YELLOW  "\033[1;93m"
#define COLOR_BLUE    "\033[1;94m"
#define COLOR_MAGENTA "\033[1;95m"
#define COLOR_RED     "\033[1;91m"

#ifdef NDEBUG
#define pretty_debug(...)
#else

// Das Pretty Debug Makro (ohne Zeitstempel)
#define pretty_debug(msg) \
    std::cerr << COLOR_BLUE << "[DEBUG] " \
              << COLOR_MAGENTA << __FILE__ << ":" << __LINE__ << " " \
              << COLOR_YELLOW << __PRETTY_FUNCTION__ << "(): " \
              << COLOR_RESET << msg << std::endl;
#endif
