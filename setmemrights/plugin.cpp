#include "plugin.h"

static bool cbCommand(int argc, char* argv[])
{
    duint addr = 0, size = 0;

    if (argc < 4 || !DbgFunctions()->ValFromString(argv[1], &addr) || !DbgFunctions()->ValFromString(argv[2], &size))
    {
        _plugin_logputs("[" PLUGIN_NAME "] Error: Use an address as arg1, size as arg2 and as arg3: Execute, ExecuteRead, ExecuteReadWrite, ExecuteWriteCopy, NoAccess, ReadOnly, ReadWrite, WriteCopy. You can add a G at first for add PAGE GUARD, example: GReadOnly\n");
        return false;
    }

    auto page_count = (size / 4096) + (size % 4096 == 0 ? 0 : 1);

    while (page_count > 0)
    {
        if (!DbgFunctions()->SetPageRights(addr, argv[3]))
            break;
        addr += 0x1000;
        page_count--;
    }

    return page_count == 0;
}

//Initialize your plugin data here.
bool pluginInit(PLUG_INITSTRUCT* initStruct)
{
    if(!_plugin_registercommand(pluginHandle, PLUGIN_NAME, cbCommand, false))
        _plugin_logputs("[" PLUGIN_NAME "] Error registering the \"" PLUGIN_NAME "\" command!");
    return true; //Return false to cancel loading the plugin.
}

//Deinitialize your plugin data here (clearing menus optional).
bool pluginStop()
{
    return true;
}

//Do GUI/Menu related things here.
void pluginSetup()
{
}
