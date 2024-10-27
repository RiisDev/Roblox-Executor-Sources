FROM amazonlinux
WORKDIR /root/app

RUN curl --silent --location https://rpm.nodesource.com/setup_16.x | bash -
RUN yum -y install nodejs
RUN yum -y install wget
RUN wget https://s3.amazonaws.com/rds-downloads/rds-combined-ca-bundle.pem

COPY api.js index.js

EXPOSE 5000
CMD ["node", "index.js"]