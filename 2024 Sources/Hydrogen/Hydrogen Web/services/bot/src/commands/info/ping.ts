import { environmentUtil } from "@hydrogen-web/utils";
import { ApplicationCommandType } from "discord.js";
import { client } from "../..";

client.registerCommand({
  name: "ping",
  description: "Ping pong!",
  type: ApplicationCommandType.ChatInput,
  run: async (client, interaction) => {
    return interaction.reply({
      content: `Pong from ${
        client.user!.tag
      }! The current environment is ${environmentUtil()}.`,
      ephemeral: true,
    });
  },
});
