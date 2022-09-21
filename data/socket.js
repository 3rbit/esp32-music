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

  socket.send(JSON.stringify({
    "target": "ping",
    "data": ""
  }));

  $("#connecting").text("Connected").attr("id", "connected").attr("class", "title has-text-success")
}

function onClose(event) {
  console.log('Connection closed');
  console.log(event);

  socket = undefined

  $("#connected").text("Connecting").attr("id", "connecting").attr("class", "title has-text-warning")

  setTimeout(initWebSocket, 2000); //attempt to reconnect
}

function onError(event) {
  console.log(event);
}

function onMessage(event) {

  let json = JSON.parse(event.data)
  let target = json["target"]
  let data = json["data"]

  switch (target) {
    case "pong":
      console.log("server ponged")
      break;

    case "sensors":
      onMessageSensor(data)
      break;

    default:
      break;
  }

  // console.log(event.data);
}

function onMessageSensor(data) {
  $(".sensor#distance").text(Math.round(data.distance * 100) / 100)
  $(".sensor#ldr").text(data.ldr)
  $(".sensor#pir").text(data.pir)
  $(".sensor#humidity").text(Math.round(data.humidity * 100) / 100)
  $(".sensor#temperature").text(Math.round(data.temperature * 100) / 100)
  $(".sensor#heatIndex").text(Math.round(data.heatIndex * 100) / 100)
}

initWebSocket()