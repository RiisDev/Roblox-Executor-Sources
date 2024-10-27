import { FastifyPluginAsync } from "fastify";
import fp from "fastify-plugin";
import { jwtVerify } from "jose";

const plugin: FastifyPluginAsync = async (fastify, options) => {
  fastify.addHook("onRequest", async (request, reply) => {
    const authHeader = request.headers.authorization || "";
    const encodedJwt = authHeader.split(" ")[1];

    try {
      const decodedJwt = await jwtVerify(
        encodedJwt,
        Buffer.from((options as any).secret),
        {
          algorithms: ["HS256"],
          clockTolerance: 10,
        }
      );

      reply.jwt = decodedJwt.payload as any;
    } catch (error) {
      reply.jwt = undefined;
    }
  });
};

declare module "fastify" {
  interface FastifyReply {
    jwt?: {
      id: string;
      roles: Array<string>;
    };
  }
}

export const jwtPlugin = fp<{
  secret: string;
}>(plugin);
