<template>
  <div class="app">
    <div class="title-bar">
      <win-title-bar v-if="platform === 'win32'" />
      <mac-title-bar v-if="platform === 'darwin' && !fullscreen" />
    </div>
    <div class="main">
      <div class="sidebar">
        <color-picker class="sb-color" />
      </div>
      <div class="canvas" @click.self="selectCanvas">
        <fixture-item
          ref="fix"
          v-for="(item, i) in 4"
          :key="i"
          @click="selectFixture"
          class="item fixture"
          :scale="canvasScale"
        />
      </div>
      <div class="sidebar" />
    </div>
  </div>
</template>

<script>
import WinTitleBar from "./components/title-bar/WinTitleBar";
import MacTitleBar from "./components/title-bar/MacTitleBar";
import FixtureItem from "./components/items/FixtureItem";
import ColorPicker from "./components/sidebar/ColorPicker";
export default {
  name: "App",
  components: {
    WinTitleBar,
    MacTitleBar,
    FixtureItem,
    ColorPicker,
  },
  data() {
    return {
      platform: "",
      fullscreen: false,
      canvasScale: 2,
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
  },

  beforeMount() {
    window.ipc.on("platform", (payload) => {
      this.platform = payload;
    });
    window.ipc.on("size", (payload) => {
      this.fullscreen = payload;
    });
    window.ipc.send("platform");
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
  font-family: Consolas, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  background-color: #2f3241;
  width: 100vw;
  height: 100vh;
  margin: 0;
}
body {
  user-select: none;
  overflow-y: hidden;
  overflow-x: hidden;
  background-color: #2f3241;
  height: 100vh;
  width: 100vw;
  margin: 0;
}
.main {
  height: 100%;
  width: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
}
.canvas {
  height: 80%;
  width: 80%;
  background-color: #14151c;
  overflow-y: hidden;
  overflow-x: hidden;
  z-index: 0;
  border: solid 2px rgb(160, 160, 160);
}
.item {
  z-index: 4;
}
.sidebar {
  width: 20%;
  height: 80%;
  background-color: rgb(63, 63, 86);
  border: solid 2px rgb(160, 160, 160);
  display: flex;
  flex-direction: column;
}
.sb-color {
  width: 100%;
  height: 25%;
}
</style>
