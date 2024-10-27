import { GetSecretValueCommand } from "@aws-sdk/client-secrets-manager";
import { environmentUtil } from "@hydrogen-web/utils";
import { secrets, secretsManager } from ".";

export const loadSecrets = async () => {
  for (const secretName of [
    "Cloudflare",
    "Database",
    "Discord",
    "Internal",
    "Linkvertise",
    "HCaptcha",
  ]) {
    const secret = await secretsManager.send(
      new GetSecretValueCommand({
        SecretId: `${environmentUtil()}/${secretName}`,
      })
    );

    secrets[secretName as keyof typeof secrets] = JSON.parse(
      secret.SecretString || ""
    );
  }
};
