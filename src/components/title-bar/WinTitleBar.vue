<template>
  <div class="top-bar">
    <div class="menu-items">
      <button class="menu-item">File</button>
      <button class="menu-item">Edit</button>
      <button class="menu-item">View</button>
      <button class="menu-item">Show</button>
      <button class="menu-item">Cue</button>
      <button class="menu-item">Fixture</button>
    </div>
    <div class="drag-title-area">
      <p>Photon — SHOW NAME</p>
    </div>
    <div class="menu-buttons">
      <button class="menu-button" @click="minimize()">
        <svg width="12" height="12" xmlns="http://www.w3.org/2000/svg">
          <line
            stroke-linecap="undefined"
            stroke-linejoin="undefined"
            y2="8"
            x2="0"
            y1="8"
            x1="18"
            stroke="#000000"
            fill="none"
          />
        </svg>
      </button>
      <button class="menu-button" @click="toggleFullscreen()">
        <svg
          v-if="!this.fullscreen"
          width="30"
          height="30"
          xmlns="http://www.w3.org/2000/svg"
        >
          <rect
            height="11"
            width="11"
            y="10"
            x="10"
            stroke="#000000"
            fill="none"
          />
        </svg>

        <svg
          v-if="this.fullscreen"
          width="30"
          height="30"
          xmlns="http://www.w3.org/2000/svg"
        >
          <rect
            height="9"
            width="9"
            y="9"
            x="12"
            stroke="#000000"
            fill="none"
          />
          <rect
            height="9"
            width="9"
            y="12"
            x="9"
            stroke="#000000"
            fill="#d9d9d9"
          />
        </svg>
      </button>
      <button class="close-button" @click="closeApp()">
        <svg
          fill="#000000"
          xmlns="http://www.w3.org/2000/svg"
          viewBox="0 0 32 32"
          width="18px"
          height="18px"
        >
          <path
            d="M 7.21875 5.78125 L 5.78125 7.21875 L 14.5625 16 L 5.78125 24.78125 L 7.21875 26.21875 L 16 17.4375 L 24.78125 26.21875 L 26.21875 24.78125 L 17.4375 16 L 26.21875 7.21875 L 24.78125 5.78125 L 16 14.5625 Z"
          />
        </svg>
      </button>
    </div>
  </div>
</template>

<script>
export default {
  name: "WinTitleBar",
  methods: {
    closeApp() {
      window.ipc.send("menu", "close");
    },
    toggleFullscreen() {
      if (!this.fullscreen) {
        window.ipc.send("menu", "fullscreenOn");
      } else {
        window.ipc.send("menu", "fullscreenOff");
      }
      this.fullscreen = !this.fullscreen;
    },
    minimize() {
      window.ipc.send("menu", "minimize");
    },
  },
  data() {
    return {
      fullscreen: false,
    };
  },
};
</script>

<style scope>
.menu-items {
  display: flex;
  margin-left: 25px;
  gap: 5px;
  align-items: center;
  justify-content: left;
  width: 33%;
}
.top-bar {
  height: 30px;
  width: 100%;
  background-color: #d9d9d9;
  display: flex;
  justify-content: space-evenly;
}
.drag-title-area {
  -webkit-app-region: drag;
  display: flex;
  align-items: center;
  justify-content: center;
  width: 33%;
  font-family: Inter;
  font-size: 0.6875rem;
  color: #000000;
}
.buffer {
  -webkit-app-region: drag;
  height: 100%;
  width: 100%;
}
.close-button {
  height: 100%;
  width: 55px;
  border: none;
  background-color: inherit;
  transition: 0.2s ease;
  display: flex;
  justify-content: center;
  align-items: center;
}
.close-button:hover {
  background-color: red;
}
.close-button:active {
  background-color: rgb(161, 0, 0);
}
.menu-buttons {
  width: 33%;
  display: flex;
  justify-content: right;
  align-items: center;
}
.menu-button {
  height: 100%;
  width: 55px;
  border: none;
  background-color: inherit;
  transition: 0.2s ease;
  display: flex;
  justify-content: center;
  align-items: center;
}
.menu-button:hover {
  background-color: #bebebe;
}
.menu-button:active {
  background-color: #a0a0a0;
}

.menu-item {
  height: 80%;
  width: auto;
  border: none;
  background-color: inherit;
  transition: 0.2s ease;
  display: flex;
  justify-content: center;
  align-items: center;
  border-radius: 25%;
  font-family: Inter;
  font-size: 0.6875rem;
}
.menu-item:hover {
  background-color: #bebebe;
}
.rest {
  font-size: 3rem;
  color: white;
}
</style>
