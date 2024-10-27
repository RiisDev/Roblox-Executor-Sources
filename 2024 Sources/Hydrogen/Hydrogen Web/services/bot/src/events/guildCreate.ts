import { secrets } from "@hydrogen-web/libraries";
import { Events } from "discord.js";
import { client } from "..";

client.registerEvent({
  name: Events.GuildCreate,
  run: async (guild) => {
    if (guild.id !== secrets.Discord.serverId) await guild.leave();
  },
});
