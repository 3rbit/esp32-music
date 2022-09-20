import { readable } from "svelte/store";

const gateway = `ws://${window.location.hostname}:3000/ws`

let socket = new WebSocket(gateway)

export const socketStore = readable(socket)

function initWebSocket() {
  if (socket == undefined)
    socket = new WebSocket(gateway);

  socket.addEventListener('open', (event) => onOpen(event));
  socket.addEventListener('close', (event) => onClose(event));
  socket.addEventListener('error', (event) => onError(event));
  socket.addEventListener('message', (event) => onMessage(event));
}

function onOpen(event: Event) {
  console.log('Connection opened');
  console.log(event);

  socket.send('ping')
}

function onClose(event: CloseEvent) {
  console.log('Connection closed');
  console.log(event);

  socket = undefined

  setTimeout(initWebSocket, 2000); //attempt to reconnect
}

function onError(event: Event) {
  console.log(event);
}

function onMessage(event: MessageEvent<any>) {

  // if (event.data == "pong") {
  //   console.log("server ponged");
    
  // }
  console.log(event.data);
}

initWebSocket()