const net = require('net');

let callback = null;
let client = null;

const arrayToAscii = arr => {
  let res = [];

  for (let i = 0; i < arr.length; i++)
    res.push(String.fromCharCode(arr[i]));

  return res.join("");
};

const server = net.createServer((c) => {
  if (client || !callback) {
    client.end();

    return;
  }

  client = c;
  console.log('client connected');
  
  c.on('end', () => {
    console.log('client disconnected');

    client = null;
  });

  c.on("error", (err) => {
    console.log('client disconnected caused by error: ' + err);

    client.end();
    client = null;
  });

  c.on('data', (data) => {
    callback(JSON.parse(arrayToAscii(data)));
  });
}); 

server.listen(8124, () => {
  console.log('server listening');
});

export default {
  setCallback: cb => {
    callback = cb;
  },

  sendData: data => {
    if (client) {
      client.write(data);
    }
  }
}