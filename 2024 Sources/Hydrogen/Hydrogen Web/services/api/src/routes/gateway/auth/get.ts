import { GetAuthRoute } from "@hydrogen-web/interfaces";
import { secrets } from "@hydrogen-web/libraries";
import { KeyModel, StateDocument } from "@hydrogen-web/models";
import { server } from "../../..";

server.route<GetAuthRoute>({
  method: "GET",
  url: "/gateway/auth",
  schema: {
    querystring: {
      type: "object",
      required: ["key", "hwid"],
      properties: {
        key: {
          type: "string",
        },
        hwid: {
          type: "string",
        },
      },
    },
  },
  handler: async (request) => {
    if (request.query.key === secrets.Internal.adminApiKey)
      return {
        data: {
          expiresAt: new Date(
            new Date().setFullYear(new Date().getFullYear() + 1)
          ),
        },
      };

    const key = await KeyModel.findOne({
      status: "ACTIVE",
      value: request.query.key,
      expiresAt: {
        $gt: new Date(),
      },
    }).populate<{ state: StateDocument }>("state");

    if (!key || !key.state) {
      if (key && !key.state) await key.delete();

      throw {
        statusCode: 401,
        message: "The specified key could not be found, please try again.",
      };
    }

    if (request.query.hwid !== key.state.hwid)
      throw {
        statusCode: 403,
        message:
          "HWID mismatch occurred, please try again or regenerate your key through the gateway.",
      };

    if (!key.hwid)
      await KeyModel.findByIdAndUpdate(key._id, {
        hwid: key.state.hwid,
      });

    return {
      data: {
        expiresAt: key.expiresAt,
      },
    };
  },
});
