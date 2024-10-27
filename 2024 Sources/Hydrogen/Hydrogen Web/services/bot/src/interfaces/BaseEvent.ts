import { ClientEvents } from "discord.js";

export interface BaseEvent<T extends keyof ClientEvents> {
  name: T;
  run: (...args: ClientEvents[T]) => void;
}
