#include "app_ready.h"

bool startScreenReady = false;
bool fontsLoaded      = false;
bool texturesLoaded   = false;
bool appReady         = false;

void updateAppReady()
{
    if (startScreenReady && fontsLoaded && texturesLoaded)
        appReady = true;
}