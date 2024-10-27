import { environmentUtil } from "./environment";

export const domainUtil = (): string => {
  return {
    development: "dev.hydrogen.sh",
    staging: "staging.hydrogen.sh",
    production: "hydrogen.sh",
  }[environmentUtil()];
};
