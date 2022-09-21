// All websocket init code goes here
// const gateway = `ws://${window.location.hostname}/ws`
const gateway = "ws://192.168.1.129/ws"

let socket = new WebSocket(gateway)


function initWebSocket() {
  if (socket == undefined)
    socket = new WebSocket(gateway);

  socket.addEventListener('open', (event) => onOpen(event));
  socket.addEventListener('close', (event) => onClose(event));
  socket.addEventListener('error', (event) => onError(event));
  socket.addEventListener('message', (event) => onMessage(event));
}

function onOpen(event) {
  console.log('Connection opened');
  console.log(event);

  socket.send('ping')
}

function onClose(event) {
  console.log('Connection closed');
  console.log(event);

  socket = undefined

  setTimeout(initWebSocket, 2000); //attempt to reconnect
}

function onError(event) {
  console.log(event);
}

function onMessage(event) {
  if (event.data == "pong") {
    console.log("server ponged")
    return
  }

  let json = JSON.parse(event.data)
  let target = json["target"]
  let data = json["data"]

  switch (target) {
    case "sensors":
      $("#json").text(JSON.stringify(data, undefined, 2))
      break;

    default:
      break;
  }

  // console.log(event.data);
}

initWebSocket()