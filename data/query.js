// All jQuery code goes here

let volumeTimer = null;
let tempoTimer = null;

function updateVolume() {
  let query = {
    "target": "updateVolume",
    "data": $("#volume").val()
  }
  socket.send(JSON.stringify(query));
}

function updateTempo() {
  let query = {
    "target": "updateTempo",
    "data": $("#tempo").val()
  }
  socket.send(JSON.stringify(query));
}

$(() => {
  $("#pingButton").click(() => {
    socket.send(JSON.stringify({
      "target": "ping",
      "data": ""
    }));
  })

  // Update volume every 50 milliseconds 
  $("#volume").on("input", () => {
    clearTimeout(volumeTimer);
    volumeTimer = setTimeout(updateVolume, 10);
  })

  $("#tempo").on("input", () => {
    clearTimeout(tempoTimer);
    tempoTimer = setTimeout(updateTempo, 10);
  })
})
