<template>
  <div class="top-bar">
    <div class="menu-items">
      <button
        v-for="option in menuOptions"
        :key="option.toLowerCase()"
        :ref="option.toLowerCase()"
        class="menu-item"
        @mouseover="show(option.toLowerCase())"
        @click="showMenu = !showMenu"
        @focusout="handleFocusOut"
      >
        {{ option }}
      </button>
      <div class="menu-items-rest" />
    </div>
    <div class="drag-title-area">
      <p>Photon — SHOW NAME</p>
    </div>
    <div class="menu-buttons">
      <div class="menu-buttons-rest" />
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
    <dropdown-menu
      ref="dropdown"
      v-show="showMenu"
      class="dropdown-menu"
      :options="options"
      @closemenu="showMenu = false"
    />
  </div>
</template>

<script>
import DropdownMenu from "../../components/dropdown/DropdownMenu";
export default {
  name: "WinTitleBar",
  components: {
    DropdownMenu,
  },
  methods: {
    closeApp() {
      if (window.ipc) window.ipc.send("menu", "close");
    },
    handleFocusOut() {
      setTimeout(() => {
        this.showMenu = false;
      }, 135);
    },
    show(menuOption) {
      const dropdown = this.$refs["dropdown"].$el;
      const element = this.$refs[menuOption][0];
      dropdown.style.left = `${element.getBoundingClientRect().x}px`;
      dropdown.style.top = `25px`;
      switch (menuOption) {
        case "file":
          this.options = [
            [
              ["New Show", () => window.ipc.send("menu", "nextCue")],
              ["Save Show", () => console.log("Save Show")],
              ["Close Show", () => console.log("Close Show")],
            ],
            [["Open Show", () => window.ipc.send("menu", "fileOpen")]],
          ];
          break;
        case "edit":
          this.options = [
            [
              ["Undo", () => console.log("Undo")],
              ["Redo", () => console.log("Redo")],
            ],
          ];
          break;
        case "view":
          this.options = [
            [
              ["Color Picker", () => console.log("Color Picker")],
              ["Intensity", () => console.log("Intensity")],
              ["Effects", () => console.log("Effects")],
            ],
            [["Edit Layout", () => console.log("Edit Layout")]],
          ];
          break;
        default:
          break;
      }
    },
    toggleFullscreen() {
      if (window.ipc) {
        if (!this.fullscreen) {
          window.ipc.send("menu", "fullscreenOn");
        } else {
          window.ipc.send("menu", "fullscreenOff");
        }
        this.fullscreen = !this.fullscreen;
      }
    },
    minimize() {
      if (window.ipc) {
        window.ipc.send("menu", "minimize");
      }
    },
  },
  data() {
    return {
      fullscreen: false,
      options: [["Hello"]],
      showMenu: false,
      menuOptions: ["File", "Edit", "View", "Show", "Cue", "Fixture"],
    };
  },
};
</script>

<style scope>
.dropdown-menu {
  position: absolute;
  z-index: 10;
}
.menu-items-rest {
  height: 30px;
  width: 100%;
  -webkit-app-region: drag;
}
.menu-items {
  display: flex;
  margin-left: 25px;
  gap: 5px;
  align-items: center;
  justify-content: left;
  width: 33%;
  flex-shrink: 0;
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
  flex-shrink: 0;
}
.close-button:hover {
  background-color: red;
}
.close-button:active {
  background-color: rgb(161, 0, 0);
}
.menu-buttons-rest {
  -webkit-app-region: drag;
  height: 30px;
  width: 100%;
}
.menu-buttons {
  width: 32.5%;
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
  flex-shrink: 0;
}
.menu-button:hover {
  background-color: #bebebe;
}
.menu-button:active {
  background-color: #a0a0a0;
}

.menu-item {
  height: 70%;
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
