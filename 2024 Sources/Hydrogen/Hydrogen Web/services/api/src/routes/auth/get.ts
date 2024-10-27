import { GetAdminAuthRoute } from "@hydrogen-web/interfaces";
import { secrets } from "@hydrogen-web/libraries";
import { server } from "../..";

server.route<GetAdminAuthRoute>({
  method: "GET",
  url: "/auth",
  schema: {
    querystring: {
      type: "object",
      required: ["apiKey"],
      properties: {
        apiKey: {
          type: "string",
        },
      },
    },
  },
  handler: async (request) => {
    if (request.query.apiKey !== secrets.Internal.adminApiKey)
      throw {
        statusCode: 401,
        message: "You do not have permission to access this route.",
      };

    return 200;
  },
});
