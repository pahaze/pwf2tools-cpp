#ifndef PWF2DISCORD_H
#define PWF2DISCORD_H
#ifdef ENABLE_RICHPRESENCE
    // Qt headers \/
    #include <QSettings>
    // Non-Qt headers \/
    #include <discord-rpc/include/discord_rpc.h>
    #include <discord-rpc/include/discord_register.h>
    #include "pwf2info.h"
    #include <string>
    #include <string.h>

    class pwf2discord {
        public:
            static bool RPCAllowed();
            static void deinitialize();
            static void initialize();
            static void updateStatus(std::string gameDetails = "", std::string gameState = "", std::string largeIconText = "pwf2tools", std::string largeIcon = "pwf2tools", std::string smallIconText = "by pahaze!", std::string smallIcon = "pahaze");
        private:
            static bool initialized;
            static DiscordEventHandlers discordHandler;
            static DiscordRichPresence richPresence;
    };
#endif
#endif // PWF2DISCORD_H
