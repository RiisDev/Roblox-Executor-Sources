import { GetReleasesRoute, ReleaseObject } from "@hydrogen-web/interfaces";
import { ReleaseModel } from "@hydrogen-web/models";
import { domainUtil } from "@hydrogen-web/utils";
import { server } from "../../../";

server.route<GetReleasesRoute>({
  method: "GET",
  url: "/releases",
  handler: async () => {
    const releases: ReleaseObject[] = [];

    for (const platform of ["WINDOWS", "ANDROID", "MACOS", "IOS"]) {
      const release = await ReleaseModel.findOne({
        latest: true,
        platform,
      });

      if (release)
        releases.push({
          url: `https://${domainUtil()}/download/${release.platform.toLowerCase()}`,
          version: release.version,
          status: release.status,
          platform: release.platform,
          createdAt: release.createdAt,
        });
    }

    return {
      data: releases,
    };
  },
});
