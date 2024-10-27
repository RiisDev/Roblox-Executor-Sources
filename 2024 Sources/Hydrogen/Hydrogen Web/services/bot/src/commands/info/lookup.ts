import {
  KeyDocument,
  KeyModel,
  StateDocument,
  StateModel,
} from "@hydrogen-web/models";
import {
  ApplicationCommandOptionType,
  ApplicationCommandType,
} from "discord.js";
import { client } from "../..";

client.registerCommand({
  name: "lookup",
  description: "Lookup a user.",
  type: ApplicationCommandType.ChatInput,
  options: [
    {
      type: ApplicationCommandOptionType.Subcommand,
      name: "key",
      description: "Lookup a key.",
      options: [
        {
          type: ApplicationCommandOptionType.String,
          name: "key",
          description: "Key to lookup.",
          required: true,
        },
      ],
    },
    {
      type: ApplicationCommandOptionType.Subcommand,
      name: "state",
      description: "Lookup a state.",
      options: [
        {
          type: ApplicationCommandOptionType.String,
          name: "token",
          description: "State to lookup.",
          required: true,
        },
      ],
    },
  ],
  run: async (_client, interaction) => {
    let result;

    if (interaction.options.getSubcommand() === "key") {
      result = await KeyModel.findOne({
        value: interaction.options.getString("key"),
      }).populate<{ state: StateDocument }>("state");

      if (!result)
        return interaction.reply({
          content: `❌ Key \`${interaction.options.getString(
            "key"
          )}\` not found, please try again.`,
          ephemeral: true,
        });

      return interaction.reply({
        ephemeral: true,
        embeds: [
          {
            title: `Key Lookup - \`${result.value}\``,
            description: `> **Status**: \`${result.status.toUpperCase()}\` ${
              {
                ACTIVE: "✅",
                DISABLED: "⛔",
                EXPIRED: "❌",
              }[result.status]
            }`,
            fields: [
              {
                name: "Key Details",
                value: `id: \`${result._id}\`\ncreatedAt: <t:${Math.floor(
                  result.createdAt.getTime() / 1000
                )}>\nexpiresAt: <t:${Math.floor(
                  result.expiresAt.getTime() / 1000
                )}> (<t:${Math.floor(result.expiresAt.getTime() / 1000)}:R>)`,
              },
              {
                name: "State Details",
                value: result.state
                  ? `status: \`${result.state.status}\`\nstage: \`${
                      result.state.stage
                    }\`\nplatform: \`${result.state.platform}\`\ntoken: \`${
                      result.state.token
                    }\`\nip: \`${result.state.ip}\`\nhwid: \`${
                      result.state.hwid
                    }\`\nuserAgent: \`${
                      result.state.userAgent
                    }\`\ncreatedAt: <t:${Math.floor(
                      result.state.createdAt.getTime() / 1000
                    )}>\nupdatedAt: <t:${Math.floor(
                      result.state.updatedAt.getTime() / 1000
                    )}>\nexpiresAt: <t:${Math.floor(
                      result.state.expiresAt.getTime() / 1000
                    )}> (<t:${Math.floor(
                      result.state.expiresAt.getTime() / 1000
                    )}:R>)`
                  : "**No state found**",
              },
            ],
          },
        ],
      });
    } else if (interaction.options.getSubcommand() === "state") {
      result = await StateModel.findOne({
        token: interaction.options.getString("token"),
      }).populate<{ key: KeyDocument }>("key");

      if (!result)
        return interaction.reply({
          content: `❌ State \`${interaction.options.getString(
            "token"
          )}\` not found, please try again.`,
          ephemeral: true,
        });

      return interaction.reply({
        ephemeral: true,
        embeds: [
          {
            title: `State Lookup - \`${result.hwid}\``,
            description: `> **Status**: \`${result.status.toUpperCase()}\` ${
              {
                WAITING: "🕐",
                PROCESSING: "🔃",
                COMPLETED: "✅",
                EXPIRED: "❌",
              }[result.status]
            }`,
            fields: [
              {
                name: "State Details",
                value: `status: \`${result.status}\`\nstage: \`${
                  result.stage
                }\`\nplatform: \`${result.platform}\`\ntoken: \`${
                  result.token
                }\`\nip: \`${result.ip}\`\nhwid: \`${
                  result.hwid
                }\`\nuserAgent: \`${
                  result.userAgent
                }\`\ncreatedAt: <t:${Math.floor(
                  result.createdAt.getTime() / 1000
                )}>\nupdatedAt: <t:${Math.floor(
                  result.updatedAt.getTime() / 1000
                )}>\nexpiresAt: <t:${Math.floor(
                  result.expiresAt.getTime() / 1000
                )}> (<t:${Math.floor(result.expiresAt.getTime() / 1000)}:R>)`,
              },
              {
                name: "Key Details",
                value: result.key
                  ? `createdAt: <t:${Math.floor(
                      result.key.createdAt.getTime() / 1000
                    )}>\nexpiresAt: <t:${Math.floor(
                      result.key.expiresAt.getTime() / 1000
                    )}> (<t:${Math.floor(
                      result.key.expiresAt.getTime() / 1000
                    )}:R>)`
                  : "**No key found**",
              },
            ],
          },
        ],
      });
    }
  },
});
