
#ifndef __STD_PARSER_H_
#define __STD_PARSER_H_

#include "Type.h"
#include "Str.h"

// Max number of nested statements
#define MAX_PARSER_STATES 4

class Parser
{
public:
    Parser(bool compileForPC = true);

    bool Process(InputStream &in, OutputStream &out);

private:
    void SyntaxError(const String &message, const String &context, int lineNumber);
    bool ProcessStatement(const String &line, OutputStream &out, int lineNumber);

private:
    bool  m_CompileForPC;                     // Do we compile for PC or XBOX
    bool  m_CurrentState[MAX_PARSER_STATES];  // The state array
    int   m_Index;                            // The current state index

};

#endif // __STD_PARSER_H_

