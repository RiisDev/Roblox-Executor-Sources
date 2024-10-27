import { login, secrets } from "@hydrogen-web/libraries";
import {
  Client,
  Collection,
  GatewayIntentBits,
  REST,
  Routes,
} from "discord.js";
import { BaseEvent, ChatInputCommand } from "./interfaces";

declare module "discord.js" {
  interface Client {
    commands: Collection<string, ChatInputCommand>;
    registerCommand: (args: ChatInputCommand) => void;
    registerEvent: <T extends keyof ClientEvents>(args: BaseEvent<T>) => void;
  }
}

export const client = new Client({ intents: [GatewayIntentBits.Guilds] });

(async () => {
  await login();

  client.commands = new Collection();

  client.registerCommand = (command) => {
    client.commands.set(command.name, command);
  };

  client.registerEvent = (event) => {
    client.on(event.name, event.run);
  };

  await import("./commands");
  await import("./events");

  if (process.argv[2] === "register") {
    const rest = new REST({ version: "10" }).setToken(secrets.Discord.botToken);
    const commands = Array.from(client.commands.values());

    try {
      console.log(
        `Started slash command refresh (${commands.length} commands)`
      );

      await rest.put(
        Routes.applicationCommands(secrets.Discord.applicationId),
        {
          body: commands,
        }
      );

      console.log("Finished slash command refresh");
    } catch (error) {
      console.error(error);
    }

    process.exit();
  }

  client.login(secrets.Discord.botToken);
})();
