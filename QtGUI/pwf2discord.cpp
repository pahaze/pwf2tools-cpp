// Non-Qt headers \/
#include "pwf2discord.h"

#ifdef ENABLE_RICHPRESENCE
    bool pwf2discord::initialized = false;
    DiscordEventHandlers pwf2discord::discordHandler;
    DiscordRichPresence pwf2discord::richPresence;

    bool pwf2discord::RPCAllowed() {
        QSettings pwf2settings(pwf2info::settingsPath, QSettings::IniFormat);
        pwf2settings.beginGroup("Privacy");
        return pwf2settings.value("EnableRPC", true).toBool();
    }

    void pwf2discord::deinitialize() {
        Discord_Shutdown();
    }

    void pwf2discord::initialize() {
        memset(&discordHandler, 0, sizeof(discordHandler));
        Discord_Initialize("776654779979399198", &discordHandler, 1, NULL);
        memset(&richPresence, 0, sizeof(richPresence));
        initialized = true;
    }

    void pwf2discord::updateStatus(std::string gameDetails, std::string gameState, std::string largeIconText, std::string largeIcon, std::string smallIconText, std::string smallIcon) {
        if(initialized) {
            if(RPCAllowed()) {
                richPresence.details = gameDetails.c_str();
                richPresence.largeImageKey = largeIcon.c_str();
                richPresence.largeImageText = largeIconText.c_str();
                richPresence.smallImageKey = smallIcon.c_str();
                richPresence.smallImageText = smallIconText.c_str();
                richPresence.state = gameState.c_str();
                Discord_UpdatePresence(&richPresence);
            } else {
                Discord_ClearPresence();
            }
        }
    }
#endif
