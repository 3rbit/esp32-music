// All jQuery code goes here

let volumeTimer = null;

function updateVolume() {
  let query = {
    "target": "updateVolume",
    "data": $("#volume").val()
  }
  socket.send(JSON.stringify(query));
}

$(() => {
  $("#pingButton").click(() => {
    console.log("button clicked");
    socket.send("ping")
  })

  $("#volume").on("input", () => {
    console.log($("#volume").val())

    clearTimeout(volumeTimer);
    volumeTimer = setTimeout(updateVolume, 50);
  })
})
