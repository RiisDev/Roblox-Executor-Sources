import { FastifyPluginAsync } from "fastify";
import fp from "fastify-plugin";

const plugin: FastifyPluginAsync = async (fastify) => {
  fastify.setErrorHandler((error, _request, reply) => {
    reply.code(error.statusCode || 500).send({
      error: {
        code: error.statusCode || 500,
        message:
          error.message || "An unexpected Internal Server Error has occurred.",
        errors: error.errors,
      },
    });
  });

  fastify.setNotFoundHandler((request) => {
    throw {
      statusCode: 404,
      message: `The specified route "${request.method}:${request.url}" could not be found.`,
    };
  });
};

declare module "fastify" {
  interface FastifyError {
    errors: unknown;
  }
}

export const errorPlugin = fp(plugin);
