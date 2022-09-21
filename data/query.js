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

function updateEnvelope() {
  let query = {
    "target": "updateEnvelope",
    "data": {
      "attackLevel": $("#attackLevel").val(),
      "decayLevel": $("decayLevel").val(),
      "sustainLevel": $("#sustainLevel").val(),
      "releaseLevel": $("#releaseLevel").val(),
      "idleLevel": $("#idleLevel").val(),
      "attackTime": $("#attackTime").val(),
      "decayTime": $("decayTime").val(),
      "sustainTime": $("#sustainTime").val(),
      "releaseTime": $("#releaseTime").val(),
      "idleTime": $("#idleTime").val(),
    }
  }
  socket.send(JSON.stringify(query))
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
    volumeTimer = setTimeout(updateVolume, 50);
  })

  $("#attackLevel, #decayLevel, #sustainLevel, #releaseLevel, #idleLevel, #attackTime, #decayTime, #sustainTime, #releaseTime, #idleTime").on("input", () => {
    clearTimeout(envelopeTimer);
    envelopeTimer = setTimeout(updateEnvelope, 50);
  })
})
