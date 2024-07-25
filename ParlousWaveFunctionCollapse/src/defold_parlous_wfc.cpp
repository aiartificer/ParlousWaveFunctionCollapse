#define LIB_NAME "ParlousWaveFunctionCollapse"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include "parlous_array.h"


static dmExtension::Result AppInitializeParlousWaveFunctionCollapse(dmExtension::AppParams* params)
{
    dmLogInfo("AppInitializeParlousWaveFunctionCollapse");
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeParlousWaveFunctionCollapse(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    dmLogInfo("Registered %s Extension", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeParlousWaveFunctionCollapse(dmExtension::AppParams* params)
{
    dmLogInfo("AppFinalizeParlousWaveFunctionCollapse");
    return dmExtension::RESULT_OK;
}

static dmExtension::Result FinalizeParlousWaveFunctionCollapse(dmExtension::Params* params)
{
    dmLogInfo("FinalizeParlousWaveFunctionCollapse");
    return dmExtension::RESULT_OK;
}

static dmExtension::Result OnUpdateParlousWaveFunctionCollapse(dmExtension::Params* params)
{
    dmLogInfo("OnUpdateParlousWaveFunctionCollapse");
    return dmExtension::RESULT_OK;
}

static void OnEventParlousWaveFunctionCollapse(dmExtension::Params* params, const dmExtension::Event* event)
{
    switch(event->m_Event)
    {
        case dmExtension::EVENT_ID_ACTIVATEAPP:
            dmLogInfo("OnEventParlousWaveFunctionCollapse - EVENT_ID_ACTIVATEAPP");
            break;
        case dmExtension::EVENT_ID_DEACTIVATEAPP:
            dmLogInfo("OnEventParlousWaveFunctionCollapse - EVENT_ID_DEACTIVATEAPP");
            break;
        case dmExtension::EVENT_ID_ICONIFYAPP:
            dmLogInfo("OnEventParlousWaveFunctionCollapse - EVENT_ID_ICONIFYAPP");
            break;
        case dmExtension::EVENT_ID_DEICONIFYAPP:
            dmLogInfo("OnEventParlousWaveFunctionCollapse - EVENT_ID_DEICONIFYAPP");
            break;
        default:
            dmLogWarning("OnEventParlousWaveFunctionCollapse - Unknown event id");
            break;
    }
}

// Defold SDK uses a macro for setting up extension entry points:
//
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)

// ParlousWaveFunctionCollapse is the C++ symbol that holds all relevant extension data.
// It must match the name field in the `ext.manifest`
DM_DECLARE_EXTENSION(ParlousWaveFunctionCollapse, LIB_NAME, AppInitializeParlousWaveFunctionCollapse, AppFinalizeParlousWaveFunctionCollapse, InitializeParlousWaveFunctionCollapse, OnUpdateParlousWaveFunctionCollapse, OnEventParlousWaveFunctionCollapse, FinalizeParlousWaveFunctionCollapse)
