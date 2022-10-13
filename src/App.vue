<template>
  <div class="app">
    <link
      rel="stylesheet"
      href="https://fonts.googleapis.com/css?family=Inter"
    />
    <div class="title-bar">
      <win-title-bar v-if="platform === 'win32'" />
      <mac-title-bar v-if="platform === 'darwin' && !fullscreen" />
    </div>
    <div class="main">
      <div class="sidebar" />
      <p-canvas></p-canvas>
    </div>
  </div>
</template>

<script>
import WinTitleBar from "./components/title-bar/WinTitleBar";
import MacTitleBar from "./components/title-bar/MacTitleBar";
import PCanvas from "./components/canvas/PCanvas";

export default {
  name: "App",
  components: {
    WinTitleBar,
    MacTitleBar,
    PCanvas,
  },
  data() {
    return {
      platform: "",
      fullscreen: false,
      canvasScale: 2,
      midiListener: null,
      value: "#ffffff",
    };
  },
  methods: {
    selectFixture(e) {
      e.preventDefault();
      e.currentTarget.classList.add("select");
    },
    selectCanvas(e) {
      e.preventDefault();
      this.$refs.fix.forEach((item) => item.$el.classList.remove("select"));
    },
    updateValue(value) {
      this.colors = value;
    },
  },

  beforeMount() {
    window.ipc.on("platform", (payload) => {
      this.platform = payload;
    });
    window.ipc.on("size", (payload) => {
      this.fullscreen = payload;
    });

    window.ipc.on("info", (payload) => {
      switch (payload.header) {
        case "resolution":
          this.screenResolution = payload.data;
          break;
      }
    });

    window.ipc.send("platform");
  },
  mounted() {
    window.ipc.send("midi");
    window.ipc.on("midi", (payload) => {
      this.midiListener = payload;
      console.log(this.midiListener);
    });
  },
  computed: {
    platformName() {
      let pName = "";
      switch (this.platform) {
        case "win32":
          pName = "Windows";
          break;
        case "darwin":
          pName = "macOS";
      }
      return pName;
    },
  },
};
</script>

<style>
.app {
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  background-color: #1d1f2b;
  width: 100vw;
  height: 100vh;
  margin: 0;
}
body {
  user-select: none;
  overflow-y: hidden;
  overflow-x: hidden;
  background-color: #1d1f2b;
  height: 100vh;
  width: 100vw;
  margin: 0;
}
.main {
  height: 100%;
  width: 100%;
  display: flex;
  align-items: center;
  justify-content: left;
}
.sidebar {
  flex-shrink: 0;
  height: 100%;
  width: 75px;
  background-color: #2f3241;
}
</style>
