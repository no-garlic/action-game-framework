
#ifndef __STD_LOG_H_
#define __STD_LOG_H_

// #define VERBOSE_LOGGING

#include "Type.h"
#include "Mem.h"
#include "Callback.h"
#include "Singleton.h"
#include "Str.h"

String LoggerTime();
String LoggerStamp(const char *file, const int line);

const char *ConvertDxErrorToString(HRESULT hr);

#ifdef _DEBUG
#ifdef DX_DETAIL_LOGGING

inline void DxLogInit()
{
    static bool init = false;
    if (init)
        return;

    init = true;

    char buffer[256];
    sprintf(buffer, "Log Started at %s\n\n", LoggerTime().c_str());

    FILE *f = fopen("dx.log", "w");
    fwrite(buffer, strlen(buffer), 1, f);
    fwrite("\n", 1, 1, f);
    fclose(f);
    fflush(f);
}

inline void DxLog(const char *file, int line, const char *msg)
{
    char buffer[256];
    sprintf(buffer, "%s (%d)\n", file, line);

    FILE *f = fopen("dx.log", "a+");
    fwrite(buffer, strlen(buffer), 1, f);
    fwrite(msg, strlen(msg), 1, f);
    fwrite("\n", 1, 1, f);
    fclose(f);
    fflush(f);
}

inline void DxLog(const char *msg)
{
    FILE *f = fopen("dx.log", "a+");
    fwrite(msg, strlen(msg), 1, f);
    fwrite("\n\n", 2, 1, f);
    fclose(f);
    fflush(f);
}

#define D3DCheck(x) { DxLogInit(); DxLog(__FILE__, __LINE__, #x); HRESULT hr = x; if (FAILED(hr)) DxLog(ConvertDxErrorToString(hr)); else DxLog("OK"); }

#else // !DX_DETAIL_LOGGING

#define D3DCheck(x) { HRESULT hr = x; if (FAILED(hr)) LOG_ERROR << ConvertDxErrorToString(hr) << ENDLOG; }

#endif // !DX_DETAIL_LOGGING

#else // !_DEBUG

// In release, we disable the directx command checking / logging.
#define D3DCheck(x) x;

#endif // !_DEBUG

class Logger
{
public:
    inline Logger();
    inline ~Logger();

    struct EndLog { };
    inline Logger &operator<<(EndLog endlog);

    inline Logger &operator<<(const char *msg);

    template <typename T>
    inline Logger &operator<<(const T &in);

    inline void AddLoggerCB(const Callback<void, const char *> &cb);

    void Out(const char *message);

private:
    StringStream                            message;
    Array<Callback<void, const char *> *>   callbacks;

};

inline Logger::Logger()
{
}

inline Logger::~Logger()
{
    for (uint i = 0; i < callbacks.size(); i++)
        delete callbacks[i];
}

template <typename T>
inline Logger &Logger::operator<<(const T &in)
{
    message << in;
    return *this;
}

inline Logger &Logger::operator<<(const char *msg)
{
    message << msg;
    return *this;
}

inline Logger &Logger::operator<<(EndLog endlog)
{
    Out(message.str().c_str());
    message.str("");
    return *this;
}

inline void Logger::AddLoggerCB(const Callback<void, const char *> &cb)
{
    callbacks.push_back(cb.Clone());
}

namespace Log
{
    extern Logger *g_plogger;
    static Logger::EndLog g_end_log;
};

#define LOG               (*Log::g_plogger)
#define LOG_INFO          (*Log::g_plogger) << "** INFO  **   "
#define LOG_WARNING       (*Log::g_plogger) << "** WARN  **   "
#define LOG_ERROR         (*Log::g_plogger) << "** ERROR **   "
#define LOG_CALLBACK(x)   Log::g_plogger->AddLoggerCB(MakeCallback(x))
#define ENDLOG            Log::g_end_log;

#ifdef FINAL_RELEASE
#define safe_cast static_cast
#else
template <typename To, typename From>
To safe_cast(From pVal)
{
    To pOut = dynamic_cast<To>(pVal);
    if (pOut == NULL)
        ASSERT(pOut, "Invalid Type Cast");
    return pOut;
}
#endif

// Object Reference Counter
//
// Usage Notes:
//
// This reference counter only operates in debug mode (when _DEBUG is defined), 
// all other times all macros are replaced by no-ops.
//
// In the constructor of any object that you want to reference count, place the
// following code (where "ClassName" is the name of the class:
// 
//      constructor(ClassName);
//
// And in the destructor place hte following code:
//
//      destructor(ClassName);
//
// When the program is finished, the ReferenceCounter will display both a log of
// the order of creation and deletion of the objects, and the current status of
// all objects it was counting.
//
// From your main() method right at the start, you should setup the reference 
// counter if the defaults do not suit you. By default no incremental logging 
// will occur, just at the end of the application, a printout of the log and 
// status will be streamed to the logger.
//
// The following macros can be used to provide specific functions:
//
// REF_COUNT_PRINT;                 - Will print the current status of all 
//                                    registered objects.
//
// REF_COUNT_LOG                    - Will print a log of all object 
//                                    creations and deletions.
//
// REF_COUNT_ENABLE_ALL(bool)       - Toggle weather or not you want all objects
//                                    automatically registered (obviously you need
//                                    the CTOR and DTOR macros in those objects).
//
// REF_COUNT_ENABLE                 - Only used when REF_COUNT_ENABLE_ALL = false, to
//                                    specify which classes to care about.
//
// REF_COUNT_NO_LOG                 - Disables an object from the log, although it
//                                    will still reference count it and print it's totals.
//
// REF_COUNT_LOG_INCREMENTAL(bool)  - Toggle weather or not you want a debug message 
//                                    each time the constructor or destructor of a  
//                                    registered class is called.
//
// REF_COUNT_LOG_DETAIL(bool)       - Do we want to log in detail every time a class
//                                    is created or destroyd?
//
// REF_COUNT_TURN_OFF               - Turn the reference counter off
//
// constructor(ClassName)        - Stick this in the constructor of any class
//
// destructor(ClassName)        - Stick this in the destructor of any class
//
// The defaults are:
//
// REF_COUNT_ENABLE_ALL(true);
// REF_COUNT_LOG_INCREMENTAL(false);
//

#ifdef _DEBUG
#  define constructor(x)                ReferenceCounterSingleton::Instance().AddInstance(#x)
#  define destructor(x)                 ReferenceCounterSingleton::Instance().RemoveInstance(#x)
#  define REF_COUNT_PRINT               ReferenceCounterSingleton::Instance().Debug()
#  define REF_COUNT_LOG                 ReferenceCounterSingleton::Instance().Log()
#  define REF_COUNT_ENABLE_ALL(x)       ReferenceCounterSingleton::Instance().SetEnableAll(x)
#  define REF_COUNT_ENABLE(x)           ReferenceCounterSingleton::Instance().Enable(#x)
#  define REF_COUNT_NO_LOG(x)           ReferenceCounterSingleton::Instance().DisableForLog(#x)
#  define REF_COUNT_LOG_INCREMENTAL(x)  ReferenceCounterSingleton::Instance().SetEnableIncrementalLog(x)
#  define REF_COUNT_LOG_DETAIL(x)       ReferenceCounterSingleton::Instance().SetFinalDetailLog(x)
#  define REF_COUNT_TURN_OFF            ReferenceCounterSingleton::Instance().Disable();
#else
#  define constructor(x)                ;
#  define destructor(x)                 ;
#  define REF_COUNT_PRINT               ;
#  define REF_COUNT_LOG                 ;
#  define REF_COUNT_ENABLE_ALL(x)       ;
#  define REF_COUNT_ENABLE(x)           ;
#  define REF_COUNT_NO_LOG(x)           ;
#  define REF_COUNT_LOG_INCREMENTAL(x)  ;
#  define REF_COUNT_LOG_DETAIL(x)       ;
#  define REF_COUNT_TURN_OFF            ;
#endif

#ifdef _DEBUG

class ReferenceCounter;

typedef SingletonHolder<ReferenceCounter, CreateStatic<ReferenceCounter> > ReferenceCounterSingleton;

class ReferenceCounter
{
public:
    void AddInstance(const String &name)
    {
        Map<String, void *>::iterator enable_iter = m_enabled_list.find(name);
        if (m_enable_all || enable_iter != m_enabled_list.end())
        {
            Map<String, int>::iterator iter = m_instances.find(name);
            if (iter == m_instances.end())
            {
                m_instances[name] = 1;
                m_max_instances[name] = 1;
                m_total_new[name] = 1;
            }
            else
            {
                m_instances[name] = iter->second + 1;
                m_max_instances[name] = std::max<int>(m_max_instances[name], m_instances[name]);
                m_total_new[name] += 1;
            }
            if (CanLog(name))
            {
                m_log.push_back(String("new:    ") + name);
            }
            if (m_enable_incremental_log && CanLog(name))
            {
                LOG << "** DEBUG ** - " << "new:    " << name << ENDLOG;
            }
        }
    }

    void RemoveInstance(const String &name)
    {
        Map<String, void *>::iterator enable_iter = m_enabled_list.find(name);
        if (m_enable_all || enable_iter != m_enabled_list.end())
        {
            Map<String, int>::iterator iter = m_instances.find(name);
            if (iter == m_instances.end())
            {
                m_instances[name] = -1;
                m_total_delete[name] = 1;
            }
            else
            {
                m_instances[name] = iter->second - 1;
                m_total_delete[name] += 1;
            }
            if (CanLog(name))
            {
                m_log.push_back(String("delete: ") + name);
            }
            if (m_enable_incremental_log && CanLog(name))
            {
                LOG << "** DEBUG ** - " << "delete: " << name << ENDLOG;
            }
        }
    }
    
    void Debug()
    {
        const int padding = 7;

        int t_cur = 0; int t_max = 0; int t_new = 0; int t_del = 0;

        LOG << "** DEBUG ** - " << "Object                               New    Del    Max    Now" << ENDLOG;
        LOG << "** DEBUG ** - " << "---------------------------          ----   ----   ----   ----" << ENDLOG;
        Map<String, int>::iterator iter;
        for (iter = m_instances.begin(); iter != m_instances.end(); ++iter)
        {
            String ref_count = iter->first;
            int current = iter->second;
            int max_instances = m_max_instances[ref_count];
            int tot_new = m_total_new[ref_count];
            int tot_del = m_total_delete[ref_count];

            t_cur += current; t_max += max_instances; t_new += tot_new; t_del += tot_del;

            ref_count.push_back(':');
            while (ref_count.size() < 29)
            {
                ref_count.push_back(' ');
            }
            LOG << "** " << (current == 0 ? "DEBUG" : "ERROR") << " ** - " << "Object: " <<
                ref_count << PadInt(tot_new, padding) << PadInt(tot_del, padding) <<
                PadInt(max_instances, padding) << PadInt(current, padding) << ENDLOG;
        }

        LOG << "** DEBUG ** - " << "---------------------------          ----   ----   ----   ----" << ENDLOG;
        LOG << "** DEBUG ** - " << "Totals:                              " <<
            PadInt(t_new, padding) << PadInt(t_del, padding) <<
            PadInt(t_max, padding) << PadInt(t_cur, padding) << ENDLOG;
    }
    
    void Log()
    {
        List<String>::iterator iter;
        for (iter = m_log.begin(); iter != m_log.end(); ++iter)
        {
            LOG << "** DEBUG ** - " << *iter << ENDLOG;
        }
    }

    void Disable()
    {
        m_enabled = false;
    }

    void SetEnableAll(bool enable_all)
    {
        m_enable_all = enable_all;
    }

    void Enable(const String &name)
    {
        m_enabled_list[name] = 0;
    }

    void SetEnableIncrementalLog(bool enable_log)
    {
        m_enable_incremental_log = enable_log;
    }

    void SetFinalDetailLog(bool log_detail)
    {
        m_final_detail_log = log_detail;
    }
    
    void DisableForLog(const String &name)
    {
        m_disabled_log_list[name] = 0;
    }

private:
    friend struct CreateStatic<ReferenceCounter>;

    ReferenceCounter()
     :  m_enabled(true),
        m_enable_all(true),
        m_enable_incremental_log(false),
        m_final_detail_log(false)
    {}

    ~ReferenceCounter()
    {
        if (!m_enabled)
            return;

        if (m_final_detail_log)
        {
            LOG << "** DEBUG ** - " << "*************** Log Of Object Creation and Deletion **************" << ENDLOG;
            Log();
        }
        LOG << "** DEBUG ** - " << "********************** Current Object Status *********************" << ENDLOG;
        Debug();
        LOG << "** DEBUG ** - " << "******************************************************************" << ENDLOG;
    }

    bool CanLog(const String &name)
    {
        return (m_disabled_log_list.find(name) == m_disabled_log_list.end());
    }
    
    String PadInt(int num, unsigned int width)
    {
        String str;
        AppendInt(str, num);

        while (str.length() < width)
            str += " ";

        return str;
    }

    Map<String, int>      m_instances;              
    Map<String, int>      m_max_instances;          
    Map<String, int>      m_total_new;              
    Map<String, int>      m_total_delete;           
    List<String>          m_log;                    
    Map<String, void *>   m_enabled_list;           
    Map<String, void *>   m_disabled_log_list;      
    bool                  m_enable_all;             
    bool                  m_enable_incremental_log; 
    bool                  m_final_detail_log;       
    bool                  m_enabled;                

};

#endif // _DEBUG

#endif // __STD_LOG_H_
