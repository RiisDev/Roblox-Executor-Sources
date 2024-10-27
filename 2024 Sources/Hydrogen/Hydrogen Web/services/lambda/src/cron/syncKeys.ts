import { login } from "@hydrogen-web/libraries";
import { KeyModel } from "@hydrogen-web/models";
import { ScheduledHandler } from "aws-lambda";

export const handler: ScheduledHandler = async () => {
  await login();

  const expiredKeys = await KeyModel.updateMany(
    {
      status: "ACTIVE",
      expiresAt: {
        $lte: new Date(),
      },
    },
    {
      status: "EXPIRED",
    }
  );

  console.info(
    `Set status of "${expiredKeys.modifiedCount}" keys to "EXPIRED"`
  );
};
