import { GetDiscordRoute } from "@hydrogen-web/interfaces";
import { secrets } from "@hydrogen-web/libraries";
import { server } from "../..";

server.route<GetDiscordRoute>({
  method: "GET",
  url: "/discord",
  handler: async (_request, reply) => {
    const invite = secrets.Discord.serverInvite;

    if (!invite) throw {};

    return reply.redirect(307, invite);
  },
});
