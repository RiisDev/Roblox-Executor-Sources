import { S3Client } from "@aws-sdk/client-s3";
import { SecretsManagerClient } from "@aws-sdk/client-secrets-manager";
import mongoose from "mongoose";
import { awsConfig } from "./config";
import { loadSecrets } from "./secrets";

export let secrets: {
  Cloudflare: {
    turnstileSecretKey: string;
    turnstileSiteKey: string;
  };
  Database: {
    username: string;
    password: string;
    host: string;
  };
  Discord: {
    serverInvite: string;
    serverId: string;
    botToken: string;
    applicationId: string;
  };
  Internal: {
    keyLifetime: number;
    stateLifetime: number;
    adCount: number;
    jwt: string;
    adminApiKey: string;
  };
  Linkvertise: {
    userId: string;
  };
  HCaptcha: {
    secretKey: string;
    siteKey: string;
  };
};

export let secretsManager: SecretsManagerClient;
export let s3: S3Client;

let loggedIn = false;

export const login = async () => {
  if (loggedIn) return;

  secrets = {} as any;
  secretsManager = new SecretsManagerClient(awsConfig);
  s3 = new S3Client(awsConfig);

  await loadSecrets();
  setInterval(loadSecrets, 60 * 15 * 1000);

  await mongoose.connect(
    `mongodb+srv://${secrets.Database.username}:${secrets.Database.password}@${secrets.Database.host}/hydrogenDB?retryWrites=true&w=majority`
  );

  loggedIn = true;
};
