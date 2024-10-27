import { FastifyPluginAsync } from "fastify";
import fp from "fastify-plugin";

const plugin: FastifyPluginAsync = async (fastify) => {
  fastify.setSchemaErrorFormatter((errors) => {
    throw {
      statusCode: 400,
      message: "Invalid or malformed data provided, please try again.",
      errors: errors.reduce((previousValue: any, currentValue: any) => {
        return [
          ...previousValue,
          {
            location:
              currentValue.keyword === "required"
                ? currentValue.params[Object.keys(currentValue.params)[0]]
                : currentValue.instancePath.split("/")[1],
            message: currentValue.message,
          },
        ];
      }, []),
    };
  });
};

export const validationPlugin = fp(plugin);
