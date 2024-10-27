import { login } from "@hydrogen-web/libraries";
import { StateModel } from "@hydrogen-web/models";
import { ScheduledHandler } from "aws-lambda";

export const handler: ScheduledHandler = async () => {
  await login();

  const expiredStates = await StateModel.updateMany(
    {
      status: {
        $nin: ["COMPLETED", "EXPIRED"],
      },
      expiresAt: {
        $lte: new Date(),
      },
    },
    {
      status: "EXPIRED",
    }
  );

  console.info(
    `Set status of "${expiredStates.modifiedCount}" states to "EXPIRED"`
  );
};
