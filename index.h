/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-controls-car-via-web
 */

const char *HTML_CONTENT = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Control Car via Web</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      margin: 0;
    }

    .container {
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    .button {
      width: 100px;
      height: 100px;
      margin: 10px;
      font-size: 18px;
      border: 2px solid #333;
      border-radius: 50%;
      background-color: #fff;
      cursor: pointer;
    }

    .button:hover {
      background-color: #f0f0f0;
    }

    h1 {
      margin-bottom: 20px;
    }

    #status {
      font-weight: bold;
      margin-top: 20px;
    }
  </style>
</head>
<body>

<div class="container">
  <h1>ESP32 - RC Car via Web</h1>
  <div>
    <button class="button" id="stop" onclick="sendCommand(0)">STOP</button>
  </div>
  <div>
    <button class="button" id="forward" onclick="sendCommand(1)">FORWARD</button>
  </div>
  <div>
    <button class="button" id="left" onclick="sendCommand(4)">LEFT</button>
    <button class="button" id="right" onclick="sendCommand(8)">RIGHT</button>
  </div>
  <div>
    <button class="button" id="backward" onclick="sendCommand(2)">BACKWARD</button>
  </div>
  <div id="status">WebSocket status: <span id="ws_status">Closed</span></div>
</div>

<script>
  var ws;

  function connectWebSocket() {
    ws = new WebSocket("ws://" + window.location.host + ":81");

    ws.onopen = function() {
      document.getElementById("ws_status").textContent = "Connected";
    };

    ws.onclose = function() {
      document.getElementById("ws_status").textContent = "Closed";
    };
  }

  function sendCommand(cmd) {
    if (ws.readyState === WebSocket.OPEN) {
      ws.send(cmd.toString());
    }
  }

  connectWebSocket();
</script>

</body>
</html>

)=====";
