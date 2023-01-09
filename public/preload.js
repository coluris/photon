const { contextBridge, ipcRenderer } = require("electron");
const fs = require("fs");
const path = require("path");
const validChannels = [
  "menu",
  "platform",
  "size",
  "midi",
  "info",
  "backend",
  "show",
  "universe",
  "fixture",
  "layout",
  "fxedit",
];
contextBridge.exposeInMainWorld("ipc", {
  send: (channel, data) => {
    if (validChannels.includes(channel)) {
      ipcRenderer.send(channel, data);
    }
  },
  on: (channel, func) => {
    if (validChannels.includes(channel)) {
      // Strip event as it includes `sender` and is a security risk
      ipcRenderer.on(channel, (event, ...args) => func(...args));
    }
  },
  openFixtureFile: (fixtureName) => {
    return fs.readFileSync(
      path.resolve(__dirname, "../Fixtures", fixtureName) + ".phix",
      "utf-8"
    );
  },
});
