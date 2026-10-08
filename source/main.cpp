#include <wups.h>
#include <notifications/notifications.h>

#include "sm3dw/hooks.h"

WUPS_PLUGIN_NAME("SM3DW Archipelago");
WUPS_PLUGIN_DESCRIPTION("Archipelago Mod for Super Mario 3D World");
WUPS_PLUGIN_VERSION("0.0.1");
WUPS_PLUGIN_AUTHOR("ItzTrackz");
WUPS_PLUGIN_LICENSE("MIT");

ON_APPLICATION_START() {
    const auto status = NotificationModule_InitLibrary();

    if (status == NOTIFICATION_MODULE_RESULT_SUCCESS) {
        NotificationModule_AddInfoNotification("SM3DW Archipelago loaded");
    }

    SM3DW::InstallHooks();
}

ON_APPLICATION_ENDS() {
    NotificationModule_DeInitLibrary();
}
