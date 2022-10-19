// All jQuery code goes here

let volumeTimer = null;
let envelopeTimer = null;

function updateVolume() {
  let query = {
    "target": "updateVolume",
    "data": $("#volume").val()
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
    // clearTimeout(volumeTimer);
    // volumeTimer = setTimeout(updateVolume, 50);
    updateVolume()
  })
})
