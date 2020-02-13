
#include "libEngine.h"
#include "ActionGameModule.h"

REGISTER_MODULE(ActionGameModule);

void LogToConsoleCallbackFunction(const char *pMsg)
{
    std::cout << pMsg << std::endl;
}

int WINAPI WinStart(const String &commandLine);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int iCmdShow)
{
#ifdef _HANDLE_EXCEPTIONS
    try {
#endif // _HANDLE_EXCEPTIONS

        String commandLine = lpCmdLine;
        int returnCode = WinStart(commandLine);

#ifdef _HANDLE_EXCEPTIONS
    }
    catch (std::exception &e)
    {
        LOG_INFO << "Exception: " << e.what() << ENDLOG;
        ::MessageBox(NULL, e.what(), "Exception", MB_OK | MB_ICONERROR);
        return -1;
    }
    catch (...)
    {
        LOG_INFO << "Unknown Exception" << ENDLOG;
        ::MessageBox(NULL, "Unknown Exception", "Exception", MB_OK | MB_ICONERROR);
        return -1;
    }
#endif 

    return returnCode;
}

int main(int argc, char *argv[])
{
    // Log to the console
    LOG_CALLBACK(LogToConsoleCallbackFunction);

    String commandLine;
    for (int i = 1; i < argc; i++)
    {
        commandLine += argv[i];
        if (i != argc - 1)
            commandLine += " ";
    }

    return WinStart(commandLine);
}

int WINAPI WinStart(const String &commandLine)
{
    // The default module to load if there is not a module specified
    String moduleName = "ActionGame";
    String normalCaseModuleName = moduleName;

    // Parse the command line
    String cmdLine = commandLine;
    if (cmdLine.length() > 0)
    {
        // Find the first white space in the command line
        int pos = static_cast<int>(cmdLine.find_first_of(' '));
        if (pos >= 0)
        {
            // Remove the module name from the command line
            moduleName = cmdLine.substr(0, pos);
            cmdLine = cmdLine.substr(pos + 1);
            normalCaseModuleName = moduleName;

            // Uppercase the module name
            ToUpper(moduleName);
        }
        else
        {
            // Remove the module name from the command line
            moduleName = cmdLine;
            cmdLine = "";
            normalCaseModuleName = moduleName;

            // Uppercase the module name
            ToUpper(moduleName);
        }
    }

    // Add the application args
    AppArgs.Append(cmdLine);

    // Create the module that we want to run
    ApplicationModule *pModule = ApplicationModuleManager::GetInstance().Create(moduleName.c_str());
    if (!pModule)
    {
        LOG_ERROR << "Failed to find the module: " << normalCaseModuleName << ENDLOG;
        return -1;
    }

    // Run the application module
    int returnCode = pModule->Main(commandLine);

    // Return the application return code
    return Application.GetExitCode() ? Application.GetExitCode() : returnCode;
};
