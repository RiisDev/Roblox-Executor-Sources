FROM public.ecr.aws/lambda/nodejs:16

WORKDIR ${LAMBDA_TASK_ROOT}
RUN npm i sharp

COPY lambda.js index.js