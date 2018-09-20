var net = require('net');
const readline = require('readline');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

var HOST = '192.168.110.128';
var PORT = 6969;

var client = new net.Socket();
client.connect(PORT, HOST, function() {

    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    // 建立连接后立即向服务器发送数据，服务器将收到这些数据 
    usage();
    cli_worker_loop(client);

});

// 为客户端添加“data”事件处理函数
// data是服务器发回的数据
client.on('data', function(data) {

    console.log('DATA: ' + data);
    

});

// 为客户端添加“close”事件处理函数
client.on('close', function() {
    console.log('Connection closed');
});

function usage() {
    console.log('lock -- lock screen');
    console.log('unlock -- unlock screen');
    console.log('exit -- exit program');
}

function cli_worker_loop(client) {
    rl.on('line', (input) => {
         console.log(input);
         if (input == 'exit')
            process.exit(0);
         else if (input == 'lock') {
            client.write(input);
         }  else if (input == 'unlock') {
            client.write(input);
        } 
      });
}