import { ActivityType, Events } from "discord.js";
import { client } from "..";

client.registerEvent({
  name: Events.ClientReady,
  run: (client) => {
    console.log(`Logged in as ${client.user.tag}`);

    client.user.setActivity({
      name: "hydrogen.sh",
      type: ActivityType.Watching,
    });
  },
});
