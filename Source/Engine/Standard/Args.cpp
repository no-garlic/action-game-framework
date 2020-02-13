
#include "Args.h"

AppArgsCache::AppArgsCache()
{
    constructor(AppArgs);
}

AppArgsCache::~AppArgsCache()
{
    destructor(AppArgs);
}

void AppArgsCache::Append(const String &options)
{
    StringArray tokens = Tokenize(options);

    StringArray args;
    String option;
    StringArray::iterator iter = tokens.begin();
    for (; iter != tokens.end(); ++iter)
    {
        const String &token = *iter;

        if (token[0] == '-')
        {
            if (option.empty())
            {
                option = token;
            }
            else
            {
                Map<String, StringArray >::iterator iter = m_options.find(option);
                if (iter == m_options.end())
                {
                    m_options.insert(MakePair(option, args));
                }
                else
                {
                    iter->second.insert(iter->second.end(), args.begin(), args.end());
                }

                option = token;
                args.clear();
            }
        }
        else
        {
            if (option.empty())
            {
                m_default_args.push_back(token);
            }
            else
            {
                args.push_back(token);
            }
        }
    }

    if (!option.empty())
    {
        Map<String, StringArray >::iterator iter = m_options.find(option);
        if (iter == m_options.end())
        {
            m_options.insert(MakePair(option, args));
        }
        else
        {
            iter->second.insert(iter->second.end(), args.begin(), args.end());
        }
    }
}

bool AppArgsCache::HasOption(const char *option)
{
    String search(option);
    ToUpper(search);

    Map<String, StringArray>::iterator iter(m_options.begin()), end(m_options.end());
    for (; iter != end; ++iter)
    {
        String opt = iter->first;
        ToUpper(opt);

        if (opt == search)
        {
            return true;
        }
    }

    return false;
}

const StringArray &AppArgsCache::GetOptionArgs(const char *option)
{
    String search(option);
    ToUpper(search);

    Map<String, StringArray>::iterator iter(m_options.begin()), end(m_options.end());
    for (; iter != end; ++iter)
    {
        String opt = iter->first;
        ToUpper(opt);

        if (opt == search)
        {
            return iter->second;
        }
    }

    static StringArray null_options;
    return null_options;
}

StringArray AppArgsCache::ListOptions()
{
    StringArray options;

    Map<String, StringArray>::iterator iter(m_options.begin()), end(m_options.end());
    for (; iter != end; ++iter)
    {
        options.push_back(iter->first);
    }

    return options;
}
