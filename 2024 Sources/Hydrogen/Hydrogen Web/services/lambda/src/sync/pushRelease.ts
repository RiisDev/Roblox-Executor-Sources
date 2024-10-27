import { CopyObjectCommand, GetObjectCommand } from "@aws-sdk/client-s3";
import { login, s3 } from "@hydrogen-web/libraries";
import { ReleaseModel } from "@hydrogen-web/models";
import { environmentUtil } from "@hydrogen-web/utils";
import { Handler } from "aws-lambda";

export const handler: Handler<{
  platform: string;
  version: string;
  type: string;
}> = async (event) => {
  await login();

  const objectResponse = await s3.send(
    new GetObjectCommand({
      Key: `${event.platform}/releases/release-${event.version}${event.type}`,
      Bucket: `hydrogen-downloads-${environmentUtil()}`,
    })
  );

  if (!objectResponse)
    throw new Error(
      `Specified release not found - ${event.platform}/${event.version}/${event.type}`
    );

  await s3.send(
    new CopyObjectCommand({
      CopySource: `hydrogen-downloads-${environmentUtil()}/${
        event.platform
      }/releases/release-${event.version}${event.type}`,
      Key: `${event.platform}/client${event.type}`,
      Bucket: `hydrogen-downloads-${environmentUtil()}`,
      ACL: "public-read",
    })
  );

  console.info(
    `Copied release to specified path successfully - ${event.platform}/client${event.type}`
  );

  await ReleaseModel.updateOne(
    {
      latest: true,
      platform: event.platform.toUpperCase(),
    },
    {
      latest: false,
      status: "PATCHED",
    }
  );

  console.info("Set previous release status to patched/NOT latest");

  const release = await ReleaseModel.create({
    latest: true,
    type: event.type,
    version: event.version,
    status: "WORKING",
    platform: event.platform.toUpperCase(),
    createdAt: new Date(),
  });

  console.info(`Created new release "${release._id}" successfully`);
};
