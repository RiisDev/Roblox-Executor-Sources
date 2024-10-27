import { PostGatewayRoute } from "@hydrogen-web/interfaces";
import { secrets } from "@hydrogen-web/libraries";
import { StateModel } from "@hydrogen-web/models";
import { domainUtil } from "@hydrogen-web/utils";
import axios from "axios";
import { randomBytes } from "node:crypto";
import { server } from "../../..";

server.route<PostGatewayRoute>({
  method: "POST",
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
    body: {
      type: "object",
      required: ["token"],
      properties: {
        token: {
          type: "string",
        },
        platform: {
          type: "string",
          enum: ["MACOS", "ANDROID"],
          // enum: ["WINDOWS", "MACOS", "ANDROID", "IOS"],
        },
      },
    },
  },
  handler: async (request, reply) => {
    const formData = new URLSearchParams();
    formData.append("secret", secrets.HCaptcha.secretKey);
    formData.append("response", request.body.token);
    formData.append("remoteip", request.headers["x-real-ip"] as string);

    const requestUrl = "https://hcaptcha.com/siteverify";
    try {
      const response = await axios({
        url: requestUrl,
        method: "POST",
        data: formData.toString(),
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
      });

      if (!response.data.success) throw new Error();
    } catch {
      throw {
        statusCode: 400,
        message: `An error occurred while validating your captcha, please try again.`,
        errors: [
          {
            location: "token",
            message: "Captcha token is not valid.",
          },
        ],
      };
    }

    if (!request.params.hwid)
      throw {
        statusCode: 400,
        message: "Invalid or malformed data provided, please try again.",
        errors: [
          {
            location: "hwid",
            message: "HWID is required.",
          },
        ],
      };

    // const existingActiveKeys = await KeyModel.countDocuments({
    //   status: "ACTIVE",
    //   hwid: request.params.hwid,
    //   expiresAt: {
    //     $gt: new Date(),
    //   },
    // });

    // if (existingActiveKeys >= 5)
    //   throw {
    //     statusCode: 403,
    //     message:
    //       "Bypass attempt detected, further attempts to circumvent the key system will result in termination of service.",
    //   };

    let state = await StateModel.findOne({
      token: request.cookies["x-state-token"],
      hwid: request.params.hwid,
      status: {
        $ne: "EXPIRED",
      },
      expiresAt: {
        $gt: new Date(),
      },
    });

    if (state)
      state = await StateModel.findByIdAndUpdate(state._id, {
        status: "PROCESSING",
        platform: state.platform
          ? undefined
          : request.body.platform
          ? request.body.platform
          : "ANDROID",
        updatedAt: new Date(),
      });
    else {
      reply.clearCookie("x-state-token", {
        domain: domainUtil(),
      });

      reply.clearCookie("x-state-token", {
        domain: domainUtil(),
        path: "/gateway",
      });

      state = await StateModel.create({
        status: "PROCESSING",
        platform: request.body.platform ? request.body.platform : "ANDROID",
        stage: 1,
        token: randomBytes(32).toString("hex"),
        ip: request.headers["x-real-ip"],
        hwid: request.params.hwid,
        userAgent: request.headers["user-agent"],
        createdAt: new Date(),
        updatedAt: new Date(),
        expiresAt: Date.now() + secrets.Internal.keyLifetime,
      });

      reply.setCookie("x-state-token", state.token, {
        domain: domainUtil(),
      });
    }

    let url = `https://linkvertise.com/${secrets.Linkvertise.userId}/hydrogen-gateway-${state?.stage}`;
    if (request.body.platform)
      url = `${url}-${request.body.platform.toLowerCase()}`;

    return {
      data: {
        stage: state!.stage,
        url,
      },
    };
  },
});
