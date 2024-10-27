import { GetReleaseRoute } from "@hydrogen-web/interfaces";
import { ReleaseModel } from "@hydrogen-web/models";
import { domainUtil } from "@hydrogen-web/utils";
import { server } from "../../../";

server.route<GetReleaseRoute>({
  method: "GET",
  url: "/releases/:platform",
  schema: {
    params: {
      type: "object",
      required: ["platform"],
      properties: {
        platform: {
          type: "string",
          enum: ["WINDOWS", "ANDROID", "MACOS", "IOS"],
        },
      },
    },
  },
  handler: async (request) => {
    const release = await ReleaseModel.findOne({
      latest: true,
      platform: request.params.platform,
    });

    if (!release)
      throw {
        statusCode: 404,
        message: "The specified release could not be found.",
      };

    return {
      data: {
        url: `https://${domainUtil()}/download/${release.platform.toLowerCase()}`,
        version: release.version,
        status: release.status,
        platform: release.platform,
        createdAt: release.createdAt,
      },
    };
  },
});
