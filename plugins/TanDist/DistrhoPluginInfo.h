#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND   "UALG"
#define DISTRHO_PLUGIN_NAME  "TanDist"
#define DISTRHO_PLUGIN_URI   "somewhere.url"
#define DISTRHO_PLUGIN_CLAP_ID "clap.TanDist.1"

#define DISTRHO_PLUGIN_HAS_UI       0
#define DISTRHO_PLUGIN_NUM_INPUTS   2
#define DISTRHO_PLUGIN_NUM_OUTPUTS  2
#define DISTRHO_PLUGIN_IS_RT_SAFE   1

enum Parameters {
    kGain,
    kParametersCount
};

#endif