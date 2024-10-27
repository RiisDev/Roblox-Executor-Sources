FROM node:16-alpine
WORKDIR /root/app

COPY bot.js index.js

CMD ["node", "index.js"]