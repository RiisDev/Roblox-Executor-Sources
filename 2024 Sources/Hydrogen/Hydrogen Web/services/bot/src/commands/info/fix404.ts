import { StateModel } from "@hydrogen-web/models";
import { domainUtil } from "@hydrogen-web/utils";
import {
  ApplicationCommandOptionType,
  ApplicationCommandType,
} from "discord.js";
import { client } from "../..";

client.registerCommand({
  name: "fix404",
  description: "Returns URL for gateway based on error token.",
  type: ApplicationCommandType.ChatInput,
  options: [
    {
      type: ApplicationCommandOptionType.String,
      name: "token",
      description: "Error token to lookup.",
      required: true,
    },
  ],
  run: async (_client, interaction) => {
    const result = await StateModel.findOne({
      token: interaction.options.getString("token"),
    });

    if (!result)
      return interaction.reply({
        content: `❌ State \`${interaction.options.getString(
          "token"
        )}\` not found, please try again.`,
        ephemeral: true,
      });

    return interaction.reply({
      content: `https://gateway.${domainUtil()}/${result.hwid}`,
    });
  },
});
