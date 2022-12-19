<template>
  <div class="app">
    <link
      rel="stylesheet"
      href="https://fonts.googleapis.com/css?family=Inter"
    />
    <div class="title-bar">
      <win-title-bar :name="showData.name ? showData.name : ''" />
      <!-- <mac-title-bar v-if="platform === 'darwin' && !fullscreen" /> -->
    </div>
    <div class="main">
      <div class="sidebar" />
      <div class="main-region">
        <div class="title">
          <h1>{{ showData.name }}</h1>
        </div>
        <div class="main-region-top-col">
          <p-canvas
            :fixtures="showData.fixtures"
            :layout="showData.layouts"
            :cuelists="showData.cuelists"
            :universes="universes"
            :selected-color="selectedColor"
          />
        </div>
        <div class="main-region-bottom-col">
          <color-picker @color="handleColorChange" />
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import WinTitleBar from "./components/title-bar/WinTitleBar";
// import MacTitleBar from "./components/title-bar/MacTitleBar";
import PCanvas from "./components/canvas/PCanvas";
import ColorPicker from "./components/attributes/ColorPicker";

export default {
  name: "App",
  components: {
    WinTitleBar,
    // MacTitleBar,
    PCanvas,
    ColorPicker,
  },
  data() {
    return {
      platform: "",
      fullscreen: false,
      canvasScale: 2,
      midiListener: null,
      value: "#ffffff",
      showData: {
        fixtures: {},
        layouts: {},
        cuelists: {},
        name: "ㅤ",
      },
      universes: {},
      selectedColor: "#FFFFFF",
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
    handleColorChange(color) {
      this.selectedColor = color;
    },
  },

  beforeMount() {
    window.ipc.on("platform", (payload) => {
      this.platform = payload;
    });
    window.ipc.on("size", (payload) => {
      this.fullscreen = payload;
    });
    window.ipc.on("show", (payload) => {
      this.showData = payload;
    });

    window.ipc.on("info", (payload) => {
      switch (payload.header) {
        case "resolution":
          this.screenResolution = payload.data;
          break;
      }
    });
    window.ipc.on("universe", (payload) => {
      this.universes = payload;
    });
    window.ipc.send("platform");
  },
  mounted() {
    window.ipc.send("midi");
    window.ipc.on("midi", (payload) => {
      this.midiListener = payload;
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
.main-region {
  width: 100%;
  height: 100%;
  display: flex;
  flex-direction: column;
  text-align: left;
  gap: 2vh;
}

.main-region-top-col {
  width: 100%;
  height: 60%;
  display: flex;
  flex-direction: row;
  align-items: center;
}

.main-region-bottom-col {
  width: 100%;
  height: 40%;
  display: flex;
  flex-direction: row;
  align-items: center;
  margin-bottom: 7vh;
}

.main-region-top-col > * {
  height: 100%;
  margin-left: 2vw;
}

.main-region-bottom-col > * {
  height: 100%;
  margin-left: 2vw;
}

.title {
  color: white;
  margin-left: 2vw;
  font-family: Inter;
  font-weight: 900;
  margin-top: 0;
}

.title > h1 {
  margin-top: 2vh;
  margin-bottom: 0;
}
</style>
