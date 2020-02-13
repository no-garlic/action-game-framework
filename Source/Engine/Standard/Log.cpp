
#include "Log.h"
#include <time.h>

namespace Log
{
#ifdef _DEBUG
    // Warning: in debug we never delete the logger, because
    // we use it to log memory leaks as the very last thing
    // before program termination.
    Logger *g_plogger = new Logger;
#else
    // In release we do not log the memory leaks, so we can
    // make the logger a static.
    static Logger g_logger;         // The logger
    Logger *g_plogger = &g_logger;  // The global endlog object
#endif
};

String LoggerTime()
{
    char ptime_str[128];
    time_t m_time_stamp = time(0);
    strftime(ptime_str, sizeof(ptime_str), "%d/%m/%y %H:%M:%S", localtime(&m_time_stamp));
    return ptime_str;
}

String LoggerStamp(const char *file, const int line)
{
    String m_file = file;
    String out;
    String file_str;

    // Remove the path from the filename
    file_str = m_file.substr(m_file.insert(
        String::size_type(0), 1, '\\').find_last_of('\\'));
    file_str = file_str.substr(file_str.insert(
        String::size_type(0), 1, '/').find_last_of('/'));

    // Add a comma
    out.append(", ");
    out.append(file_str.substr(file_str.find_first_not_of("\\/")));

    // Add the line
    out.append(" [");
    char str_line[32];
    sprintf(str_line, "%d", line);
    out.append(str_line);
    out.append("] - ");

    // Return the string
    return out;
}

void Logger::Out(const char *message)
{
#ifdef _DEBUG
    _RPT1(0, "%s\n", message);
#endif // _DEBUG

    for (uint i = 0; i < callbacks.size(); i++)
        (*callbacks[i])(message);
}
