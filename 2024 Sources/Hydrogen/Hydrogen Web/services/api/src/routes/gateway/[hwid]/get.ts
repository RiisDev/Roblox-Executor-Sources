import { GetGatewayRoute } from "@hydrogen-web/interfaces";
import { KeyDocument, KeyModel, StateModel } from "@hydrogen-web/models";
import { domainUtil } from "@hydrogen-web/utils";
import axios from "axios";
import { server } from "../../..";

server.route<GetGatewayRoute>({
  method: "GET",
  url: "/gateway/:hwid",
  schema: {
    params: {
      type: "object",
      required: ["hwid"],
      properties: {
        hwid: {
          type: "string",
        },
      },
    },
  },
  handler: async (request, reply) => {
    const state = await StateModel.findOne({
      token: request.cookies["x-state-token"],
      hwid: request.params.hwid,
      status: {
        $ne: "EXPIRED",
      },
      expiresAt: {
        $gt: new Date(),
      },
    }).populate<{ key: KeyDocument }>({
      path: "key",
      match: {
        status: "ACTIVE",
        expiresAt: {
          $gt: new Date(),
        },
      },
    });

    if (!state || (state.status === "COMPLETED" && !state.key)) {
      reply.clearCookie("x-state-token", {
        domain: domainUtil(),
      });

      reply.clearCookie("x-state-token", {
        domain: domainUtil(),
        path: "/gateway",
      });

      throw {
        statusCode: 404,
        message:
          'The specified resource could not be found, error: "state does not exist."',
      };
    }

    if (state.key && !state.key.hwid)
      await KeyModel.findByIdAndUpdate((state.key as any)._id.toString(), {
        hwid: state.hwid,
      });

    return {
      data: {
        stage: state.stage,
        key: state.key
          ? {
              value: state.key.value,
              expiresAt: state.key.expiresAt,
            }
          : undefined,
      },
    };
  },
});
