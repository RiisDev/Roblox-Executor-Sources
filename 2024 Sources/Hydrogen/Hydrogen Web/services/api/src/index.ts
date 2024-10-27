import fastifyCookie from "@fastify/cookie";
import fastifyCors from "@fastify/cors";
import fastifyMultipart from "@fastify/multipart";
import { login } from "@hydrogen-web/libraries";
import { domainUtil, environmentUtil } from "@hydrogen-web/utils";
import fastify from "fastify";
import { validationPlugin } from "./plugins/validation";
import { errorPlugin } from "./plugins/error";

export const server = fastify({
  logger: true,
  ajv: {
    customOptions: {
      allErrors: true,
    },
  },
});

(async () => {
  await login();

  await server.register(errorPlugin);
  await server.register(validationPlugin);
  await server.register(fastifyCookie);
  await server.register(fastifyMultipart);
  // await server.register(jwtPlugin, {
  //   secret: secrets.Internal.jwt, aa
  // });
  await server.register(fastifyCors, {
    origin: [
      `https://${domainUtil()}`,
      `https://dashboard.${domainUtil()}`,
      `https://gateway.${domainUtil()}`,
    ],
    credentials: true,
  });

  await import("./routes");

  await server.listen({
    port: 5000,
    host: environmentUtil() === "development" ? "127.0.0.1" : "0.0.0.0",
  });
})();
