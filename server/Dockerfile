FROM node
COPY package.* /server/
WORKDIR /server
RUN npm i
COPY . /server
CMD node index.js
EXPOSE 80
