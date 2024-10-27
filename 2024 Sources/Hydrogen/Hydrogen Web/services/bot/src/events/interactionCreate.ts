import { Events } from "discord.js";
import { client } from "..";
import { ChatInputCommand } from "../interfaces";

client.registerEvent({
  name: Events.InteractionCreate,
  run: async (interaction) => {
    if (!interaction.isChatInputCommand()) return;

    if (!interaction.client.commands.has(interaction.commandName)) return;

    try {
      const command: ChatInputCommand = interaction.client.commands.get(
        interaction.commandName
      ) as ChatInputCommand;

      command.run(interaction.client, interaction);
    } catch (error) {
      console.error(error);

      await interaction.reply({
        content:
          "Something went wrong while handling this command, please try again.",
        ephemeral: true,
      });
    }
  },
});
