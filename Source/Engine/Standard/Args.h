
#ifndef __STD_ARGS_H_
#define __STD_ARGS_H_

#include "Type.h"
#include "Mem.h"
#include "Str.h"
#include "Log.h"

class AppArgsCache;
typedef SingletonHolder<AppArgsCache, CreateUsingNew<AppArgsCache> > AppArgsSingleton;
#define AppArgs AppArgsSingleton::Instance()

class AppArgsCache : private Unique
{
public:
    AppArgsCache();
    ~AppArgsCache();

    void Append(const String &options);

    bool HasOption(const char *option);
    bool operator()(const char *option) { return HasOption(option); }

    const StringArray &GetOptionArgs(const char *option);
    const StringArray &GetDefaultArgs() { return m_default_args; }

    bool IsEmpty() const { return m_options.empty() && m_default_args.empty(); }
    StringArray ListOptions();

private:
    Map<String, StringArray>    m_options;
    StringArray                 m_default_args;

};

#endif // __STD_ARGS_H_
