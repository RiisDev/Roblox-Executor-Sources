import { server } from "../..";
import { GetHealthCheckRoute } from "@hydrogen-web/interfaces";

server.route<GetHealthCheckRoute>({
  method: "GET",
  url: "/healthcheck",
  handler: async (request) => {
    return `${request.hostname} is healthy.`;
  },
});
