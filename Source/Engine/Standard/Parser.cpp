
#include "Parser.h"
#include "Log.h"

// Tokens
namespace ParserToken
{
    static const String IF_PC = "#if PC";
    static const String IF_XBOX = "#if XBOX";
    static const String ELSE = "#else";
    static const String ENDIF = "#endif";
};

Parser::Parser(bool compileForPC)
{
    memset(m_CurrentState, 0, sizeof(bool) * MAX_PARSER_STATES);
    m_Index = 0;
    m_CurrentState[0] = true;
    m_CompileForPC = compileForPC;
}

bool Parser::Process(InputStream &in, OutputStream &out)
{
    // Check that the input stream is valid
    if (in.bad() || in.fail() || in.eof())
        return false;

    // Add all lines to the input string array
    int lineNumber = 0;
    bool exitLoop = false;
    while (!exitLoop)
    {
        // Get the next line of the stream
        String line;
        std::getline(in, line);
        lineNumber++;

        // Process the line
        if (!line.empty())
        {
            if (!ProcessStatement(line, out, lineNumber))
                return false;
        }
        else if (m_CurrentState[m_Index])
        {
            out << std::endl;
            //LOG_INFO << ENDLOG;
        }

        // Exit the loop when we reach the end of the stream
        if (in.eof() || !in.good())
            exitLoop = true;
    }

    if (m_Index != 0)
    {
        SyntaxError("#if found without matching #endif at the end of the file", "EOF", lineNumber);
        return false;
    }

    return true;
}

bool Parser::ProcessStatement(const String &line, OutputStream &out, int lineNumber)
{
    String stmt = line;
    TrimString(stmt);

    if (stmt.empty())
    {
        out << line << std::endl;
        return true;
    }

    if (stmt.length() > 0 && stmt.at(0) == '#')
    {
        if (stmt == ParserToken::IF_PC)
        {
            m_Index++;
            m_CurrentState[m_Index] = m_CompileForPC;
        }
        else if (stmt == ParserToken::IF_XBOX)
        {
            m_Index++;
            m_CurrentState[m_Index] = !(m_CompileForPC);
        }
        else if (stmt == ParserToken::ELSE)
        {
            m_CurrentState[m_Index] = !(m_CurrentState[m_Index]);
        }
        else if (stmt == ParserToken::ENDIF)
        {
            if (m_Index == 0)
            {
                SyntaxError("#endif found without matching #if", line, lineNumber);
                return false;
            }
            else if (m_Index == MAX_PARSER_STATES - 1)
            {
                SyntaxError("stack overflow", line, lineNumber);
                return false;
            }
            m_Index--;
            return true;
        }
        else
        {
            SyntaxError("statement is invalid", line, lineNumber);
            return false;
        }
    }
    else if (m_CurrentState[m_Index])
    {
        out << line << std::endl;
        //LOG_INFO << line << ENDLOG;
        return true;
    }

    return true;
}

void Parser::SyntaxError(const String &message, const String &context, int lineNumber)
{
    LOG_ERROR << "Syntax error at line " << lineNumber << " - " << message << ENDLOG;
    LOG_ERROR << context << ENDLOG;
}
