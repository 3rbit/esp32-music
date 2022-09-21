// All jQuery code goes here

$(() => {
  $("#pingButton").click(() => {
    console.log("button clicked");
    socket.send("ping")
  })
})