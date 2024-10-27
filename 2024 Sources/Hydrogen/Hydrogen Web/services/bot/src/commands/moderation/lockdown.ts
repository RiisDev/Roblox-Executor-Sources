import {
  ApplicationCommandOptionType,
  ApplicationCommandType,
  CategoryChannel,
  CategoryChildChannel,
  ChannelType,
  Collection,
} from "discord.js";
import { client } from "../..";

client.registerCommand({
  name: "lockdown",
  description: "Start/stop a lockdown.",
  type: ApplicationCommandType.ChatInput,
  options: [
    {
      type: ApplicationCommandOptionType.Subcommand,
      name: "start",
      description: "Starts a lockdown.",
    },
    {
      type: ApplicationCommandOptionType.Subcommand,
      name: "end",
      description: "Ends a lockdown.",
    },
  ],
  run: async (_client, interaction) => {
    const memberRole = interaction.guild?.roles.cache.find(
      (role) => role.name === "Member"
    );

    if (!memberRole)
      return interaction.reply({
        content: "❌ Member role not found.",
        ephemeral: true,
      });

    const publicCategories = ["general", "scripts"];
    const publicChannels = ["public-support"];

    let channels: Collection<string, CategoryChildChannel> = new Collection();

    for (const categoryName of publicCategories) {
      const categoryChannel = interaction.guild?.channels.cache.find(
        (channel) =>
          channel.type === ChannelType.GuildCategory &&
          channel.name === categoryName
      ) as CategoryChannel;

      if (!categoryChannel) continue;

      channels = channels.concat(categoryChannel.children.cache);
    }

    for (const channelName of publicChannels) {
      const textChannel = interaction.guild?.channels.cache.find(
        (channel) =>
          channel.type === ChannelType.GuildText && channel.name === channelName
      );

      if (!textChannel) continue;

      channels.set(textChannel.id, textChannel as CategoryChildChannel);
    }

    if (interaction.options.getSubcommand() === "start") {
      for (const channel of channels) {
        await channel[1].permissionOverwrites.create(memberRole, {
          SendMessages: false,
        });
      }

      return interaction.reply({
        content: `🔒 Lockdown started successfully. (channels: ||${channels.map(
          (channel) => `<#${channel.id}>`
        )}||)`,
      });
    } else if (interaction.options.getSubcommand() === "end") {
      for (const channel of channels) {
        await channel[1].lockPermissions();
      }

      return interaction.reply({
        content: `🔓 Lockdown ended successfully. (channels: ||${channels.map(
          (channel) => `<#${channel.id}>`
        )}||)`,
      });
    }
  },
});
