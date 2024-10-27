import { GetCallbackRoute } from "@hydrogen-web/interfaces";
import { secrets } from "@hydrogen-web/libraries";
import { KeyModel, StateModel } from "@hydrogen-web/models";
import { domainUtil, environmentUtil } from "@hydrogen-web/utils";
import { randomBytes } from "node:crypto";
import { server } from "../../..";

server.route<GetCallbackRoute>({
  method: "GET",
  url: "/gateway/callback",
  schema: {
    querystring: {
      type: "object",
      required: ["stage"],
      properties: {
        stage: {
          type: "string",
        },
        token: {
          type: "string",
        },
      },
    },
  },
  handler: async (request, reply) => {
    const stage = parseInt(request.query.stage);

    if (!stage || stage % 1 !== 0 || stage > secrets.Internal.adCount)
      throw {
        statusCode: 404,
        message: `The specified route "${request.method}:${request.url}" could not be found.`,
      };

    const state = await StateModel.findOne({
      key: null,
      status: "PROCESSING",
      token: request.cookies["x-state-token"],
      expiresAt: {
        $gt: new Date(),
      },
    });

    if (!state)
      return reply.redirect(
        `https://gateway.${domainUtil()}?error=STATE_NOT_FOUND&token=${
          request.cookies["x-state-token"] || "none"
        }`
      );

    if (
      environmentUtil() !== "development" &&
      (!request.headers.referer ||
        request.headers.referer !== "https://linkvertise.com/" ||
        Date.now() - state.updatedAt.getTime() < 15 * 1000 ||
        stage !== state.stage)
    )
      throw {
        statusCode: 403,
        message:
          "Bypass attempt detected, further attempts to circumvent the key system will result in termination of service.",
      };

    let key;

    if (stage + 1 > secrets.Internal.adCount)
      key = await KeyModel.create({
        state: state._id,
        status: "ACTIVE",
        platform: state.platform ? state.platform : "ANDROID",
        value: randomBytes(16).toString("hex"),
        hwid: state.hwid,
        expiresAt: Date.now() + secrets.Internal.keyLifetime,
        createdAt: new Date(),
      });

    await state.update({
      key: key ? key._id : undefined,
      status: stage + 1 > secrets.Internal.adCount ? "COMPLETED" : "WAITING",
      updatedAt: new Date(),
      $inc: {
        stage: 1,
      },
    });

    let url = `https://gateway.${domainUtil()}/${state.hwid}`;
    if (state.platform && state.platform !== "ANDROID")
      url = `${url}?platform=${state.platform}`;

    return reply.redirect(url);
  },
});
