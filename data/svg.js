function scale(number, inMin, inMax, outMin, outMax) {
  return (number - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

var draw = SVG().addTo('#envelopeGraph').viewbox(0, 0, 2000, 255)

let attackY = scale($("#attackLevel").val(), 0, 255, 255, 0);
var attackLine = draw.line(0, 255, $("#attackTime").val(), attackY)
attackLine.stroke({ color: '#f06', width: 5, linecap: 'round' })

let decayY = scale($("#decayLevel").val(), 0, 255, 255, 0);
let decayOffsetX = parseInt($("#attackTime").val())
var decayLine = draw.line(decayOffsetX, attackY, decayOffsetX + parseInt($("#decayTime").val()), decayY)
decayLine.stroke({ color: '#f06', width: 5, linecap: 'round' })

let sustainY = scale($("#sustainLevel").val(), 0, 255, 255, 0);
let sustainOffsetX = decayOffsetX + parseInt($("#decayTime").val())
var sustainLine = draw.line(sustainOffsetX, decayY, sustainOffsetX + parseInt($("#sustainTime").val()), sustainY)
sustainLine.stroke({ color: '#f06', width: 5, linecap: 'round' })

let releaseY = scale($("#releaseLevel").val(), 0, 255, 255, 0);
let releaseOffsetX = sustainOffsetX + parseInt($("#sustainTime").val())
var releaseLine = draw.line(releaseOffsetX, sustainY, releaseOffsetX + parseInt($("#releaseTime").val()), releaseY)
releaseLine.stroke({ color: '#f06', width: 5, linecap: 'round' })


draw.on("updateEvent", () => {
  let attackY = scale($("#attackLevel").val(), 0, 255, 255, 0);
  attackLine.plot(0, 255, $("#attackTime").val(), attackY)

  let decayY = scale($("#decayLevel").val(), 0, 255, 255, 0);
  let decayOffsetX = parseInt($("#attackTime").val())
  decayLine.plot(decayOffsetX, attackY, decayOffsetX + parseInt($("#decayTime").val()), decayY)

  let sustainY = scale($("#sustainLevel").val(), 0, 255, 255, 0);
  let sustainOffsetX = decayOffsetX + parseInt($("#decayTime").val())
  sustainLine.plot(sustainOffsetX, decayY, sustainOffsetX + parseInt($("#sustainTime").val()), sustainY)

  let releaseY = scale($("#releaseLevel").val(), 0, 255, 255, 0);
  let releaseOffsetX = sustainOffsetX + parseInt($("#sustainTime").val())
  releaseLine.plot(releaseOffsetX, sustainY, releaseOffsetX + parseInt($("#releaseTime").val()), releaseY)
})
