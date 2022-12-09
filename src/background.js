"use strict";

import { app, protocol, BrowserWindow, screen, dialog } from "electron";
import { createProtocol } from "vue-cli-plugin-electron-builder/lib";
import installExtension, { VUEJS3_DEVTOOLS } from "electron-devtools-installer";
import { SerialPort } from "serialport";

const fs = require("fs");
const path = require("path");
const { ipcMain } = require("electron");
const isDevelopment = process.env.NODE_ENV !== "production";
const midi = require("midi");
const midiListener = new midi.Input();
const backend = require("child_process");
const FRAME_RATE = 40;
const runner = backend.spawn("PhotonDMXHandler", {
  stdio: ["pipe", "pipe", "pipe"],
});
let showData = {};
let universeData = {};

// Scheme must be registered before the app is ready
protocol.registerSchemesAsPrivileged([
  { scheme: "app", privileges: { secure: true, standard: true } },
]);
async function createWindow() {
  // Create the browser window.
  const win = new BrowserWindow({
    width: 1200,
    height: 675,
    minWidth: 960,
    minHeight: 540,
    titleBarStyle: "hidden",
    icon: "public/icon.ico",
    webPreferences: {
      nodeIntegration: process.env.ELECTRON_NODE_INTEGRATION,
      contextIsolation: !process.env.ELECTRON_NODE_INTEGRATION,
      enableRemoteModule: false,
      preload: path.resolve(__static, "preload.js"),
    },
  });

  if (process.env.WEBPACK_DEV_SERVER_URL) {
    // Load the url of the dev server if in development mode
    await win.loadURL(process.env.WEBPACK_DEV_SERVER_URL);
    if (!process.env.IS_TEST) win.webContents.openDevTools();
  } else {
    createProtocol("app");
    // Load the index.html when not in development
    win.loadURL("app://./index.html");
  }

  ipcMain.on("menu", (event, payload) => {
    switch (payload) {
      case "close":
        app.exit(0);
        break;
      case "minimize":
        win.minimize();
        break;
      case "fullscreenOn":
        win.setFullScreen(true);
        break;
      case "fullscreenOff":
        win.setFullScreen(false);
        break;
      case "isFullscreen":
        event.reply("menu", win.isFullScreen());
        break;
      case "fileOpen": {
        const showFileDialog = dialog.showOpenDialogSync({
          properties: ["openFile"],
          filters: [{ name: "Photon Show Files", extensions: ["pshow"] }],
        });
        if (showFileDialog === undefined) {
          break;
        }
        const showFile = showFileDialog[0];
        showData = JSON.parse(fs.readFileSync(showFile, "utf8"));
        win.webContents.send("show", showData);
        sendProcMessage("ldshow", showFile);
        sendProcMessage("artnet", "169.254.158.174");
        break;
      }
      case "nextCue":
        sendProcMessage("nextcue");
        break;
    }
  });
  ipcMain.on("fixture", (event, payload) => {
    sendProcMessage(
      "fedit",
      `${payload.fixtures}|${payload.attr}|${payload.value}`
    );
  });
  win.on("enter-full-screen", () => {
    win.webContents.send("size", win.isFullScreen());
  });

  win.on("leave-full-screen", () => {
    win.webContents.send("size", win.isFullScreen());
  });
  await SerialPort.list().then((/*ports*/) => {
    // console.log("ports", ports);
  });

  runner.stderr.on("data", (data) => {
    console.log(data.toString());
  });

  runner.stdout.on("data", (data) => {
    const messages = data.toString().split("\n").slice(0, -1);
    for (const x in messages) {
      if (messages[x].slice(0, 3) === "@p!") {
        const command = messages[x].split("@p!")[1].split(";")[0];
        processCommand(command);
      } else {
        console.log(messages[x]);
      }
    }
  });
  function processCommand(command) {
    try {
      if (command.slice(0, 3) === "pkt") {
        const cmd_split = command.split("]");
        const universe = parseInt(cmd_split[0].split("[")[1], 10);
        const data = cmd_split[1].split(",").map((x) => parseInt(x, 10));
        universeData[universe] = data;
        if (universe === 0) {
          win.webContents.send("universe", universeData);
        }
      }
    } catch (e) {
      // do nothing lmao
    }
  }
  initBackend();
}

ipcMain.on("info", (event, payload) => {
  switch (payload) {
    case "resolution":
      event.reply("info", {
        header: "resolution",
        data: {
          width: screen.availWidth,
          height: screen.availHeight,
        },
      });
      break;
  }
});

ipcMain.on("midi", (event) => {
  // console.log(midiListener.getPortName(0));
  event.reply("midi", midiListener.getPortCount());
});

ipcMain.on("backend", (event, payload) => {
  sendProcMessage(payload.command, payload.data);
});

ipcMain.on("platform", (event) => {
  event.reply("platform", process.platform);
});

// Quit when all windows are closed.
app.on("window-all-closed", () => {
  // On macOS it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  runner.stdout.end();
  runner.stdin.end();
  runner.kill();
  app.quit();
});

app.on("activate", () => {
  // On macOS it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (BrowserWindow.getAllWindows().length === 0) createWindow();
});

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on("ready", async () => {
  if (isDevelopment && !process.env.IS_TEST) {
    // Install Vue Devtools
    try {
      await installExtension(VUEJS3_DEVTOOLS);
    } catch (e) {
      console.error("Vue Devtools failed to install:", e.toString());
    }
  }
  createWindow();
});

// Exit cleanly on request from parent process in development mode.
if (isDevelopment) {
  if (process.platform === "win32") {
    process.on("message", (data) => {
      if (data === "graceful-exit") {
        runner.stdout.end();
        runner.stdin.end();
        runner.kill();
        app.quit();
      }
    });
  } else {
    process.on("SIGTERM", () => {
      runner.stdout.end();
      runner.stdin.end();
      runner.kill();
      app.quit();
    });
  }
}

function initBackend() {
  sendProcMessage("frate", FRAME_RATE.toString());
  sendProcMessage("uadd", "4");
  sendProcMessage("uedit", "0|a");
}

function sendProcMessage(command, data) {
  const format = "@p!" + command + "~" + data + "\n";
  runner.stdin.write(format);
}
