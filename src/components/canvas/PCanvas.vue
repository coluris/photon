<template>
  <div
    class="canvas"
    id="canvas"
    @mousedown.self="handleMouseDown"
    @mouseup="handleMouseUp"
    @mousemove="handleMouseMove"
    @mouseleave.self="handleMouseUp"
  >
    <div class="fixtures" v-if="fixtures">
      <fixture-item
        v-for="x in Object.keys(fixtures)"
        :key="x"
        :id="x"
        :data="{
          color: getFixtureColorVals(x),
          posX: layout['Layout 1'][x].x,
          posY: layout['Layout 1'][x].y,
        }"
        :class="getClassname(x)"
        @click="selectedFixtures.push(x)"
      />
      <selection-box
        v-if="isMultiSelectEnabled"
        :start-x="selectStart.x"
        :start-y="selectStart.y"
        :end-x="selectEnd.x"
        :end-y="selectEnd.y"
      />
    </div>
    <input type="color" @input="handleColorChange" />
  </div>
</template>

<script>
import FixtureItem from "../items/FixtureItem";
import SelectionBox from "../tools/SelectionBox";

export default {
  name: "PCanvas",
  components: {
    FixtureItem,
    SelectionBox,
  },
  data() {
    return {
      selectedFixtures: [],
      isMultiSelectEnabled: false,
      selectStart: {
        x: 0,
        y: 0,
      },
      selectEnd: {
        x: 0,
        y: 0,
      },
    };
  },
  computed: {
    fixListAsString() {
      let output = "";
      for (const fix in this.selectedFixtures) {
        output += `${this.selectedFixtures[fix]},`;
      }
      return output;
    },
    selectionBox() {
      return {
        top: Math.min(this.selectStart.y, this.selectEnd.y),
        bottom: Math.max(this.selectStart.y, this.selectEnd.y),
        left: Math.min(this.selectStart.x, this.selectEnd.x),
        right: Math.max(this.selectStart.x, this.selectEnd.x),
      };
    },
  },
  methods: {
    isSelecting(fix) {
      // LOL FIX LATER PLEASE
      return (
        (fix.offsetTop >= this.selectionBox.top &&
          fix.offsetLeft >= this.selectionBox.left &&
          fix.offsetTop <= this.selectionBox.bottom &&
          fix.offsetLeft <= this.selectionBox.right) ||
        (fix.offsetTop + fix.offsetHeight >= this.selectionBox.top &&
          fix.offsetLeft >= this.selectionBox.left &&
          fix.offsetTop + fix.offsetHeight <= this.selectionBox.bottom &&
          fix.offsetLeft <= this.selectionBox.right) ||
        (fix.offsetTop >= this.selectionBox.top &&
          fix.offsetLeft + fix.offsetWidth >= this.selectionBox.left &&
          fix.offsetTop <= this.selectionBox.bottom &&
          fix.offsetLeft + fix.offsetWidth <= this.selectionBox.right) ||
        (fix.offsetTop + fix.offsetHeight >= this.selectionBox.top &&
          fix.offsetLeft + fix.offsetWidth >= this.selectionBox.left &&
          fix.offsetTop + fix.offsetHeight <= this.selectionBox.bottom &&
          fix.offsetLeft + fix.offsetWidth <= this.selectionBox.right) ||
        (fix.offsetTop >= this.selectionBox.top &&
          fix.offsetTop <= this.selectionBox.bottom &&
          fix.offsetLeft <= this.selectionBox.left &&
          fix.offsetLeft + fix.offsetWidth >= this.selectionBox.right) ||
        (fix.offsetTop + fix.offsetHeight >= this.selectionBox.top &&
          fix.offsetTop + fix.offsetHeight <= this.selectionBox.bottom &&
          fix.offsetLeft <= this.selectionBox.left &&
          fix.offsetLeft + fix.offsetWidth >= this.selectionBox.right) ||
        (fix.offsetTop <= this.selectionBox.top &&
          fix.offsetTop + fix.offsetHeight >= this.selectionBox.bottom &&
          fix.offsetLeft >= this.selectionBox.left &&
          fix.offsetLeft <= this.selectionBox.right) ||
        (fix.offsetTop <= this.selectionBox.top &&
          fix.offsetTop + fix.offsetHeight >= this.selectionBox.bottom &&
          fix.offsetLeft + fix.offsetWidth >= this.selectionBox.left &&
          fix.offsetLeft + fix.offsetWidth <= this.selectionBox.right)
      );
    },
    getClassname(fixtureID) {
      if (this.selectedFixtures.includes(fixtureID)) {
        return "select";
      }
      return "";
    },
    handleMouseUp() {
      this.isMultiSelectEnabled = false;
      this.selectStart = { x: 0, y: 0 };
      this.selectEnd = { x: 0, y: 0 };
    },
    handleMouseDown(event) {
      this.isMultiSelectEnabled = true;
      const canvas = document.getElementById("canvas");
      const x = event.clientX - canvas.offsetLeft;
      const y = event.clientY - canvas.offsetTop;
      this.selectStart = { x, y };
      this.selectEnd = { x, y };
    },
    handleMouseMove(event) {
      if (this.isMultiSelectEnabled) {
        const canvas = document.getElementById("canvas");
        const x = event.clientX - canvas.offsetLeft;
        const y = event.clientY - canvas.offsetTop;
        this.selectEnd = { x, y };
        this.computeMultiSelectFixtures(event);
      }
    },
    computeMultiSelectFixtures(event) {
      const fixtures = document.getElementsByClassName("fix");
      const selected = [];
      for (const fix of fixtures) {
        if (this.isSelecting(fix)) {
          selected.push(fix.id);
        }
      }
      if (event.ctrlKey) {
        const mergedSet = new Set(selected.concat(this.selectedFixtures));
        this.selectedFixtures = Array.from(mergedSet);
      } else {
        this.selectedFixtures = selected;
      }
    },
    handleColorChange(event) {
      const red = parseInt(event.srcElement.value.substring(1, 3), 16);
      const green = parseInt(event.srcElement.value.substring(3, 5), 16);
      const blue = parseInt(event.srcElement.value.substring(5, 7), 16);

      if (this.selectedFixtures.length !== 0) {
        window.ipc.send("fixture", {
          fixtures: this.fixListAsString,
          attr: "r",
          value: red,
        });
        window.ipc.send("fixture", {
          fixtures: this.fixListAsString,
          attr: "g",
          value: green,
        });
        window.ipc.send("fixture", {
          fixtures: this.fixListAsString,
          attr: "b",
          value: blue,
        });
      }
    },
    getFixtureColorVals(fix_id) {
      const fixtureData = JSON.parse(
        window.ipc.openFixtureFile(this.fixtures[fix_id].type)
      );
      const redChannel = parseInt(fixtureData["channels"]["r"], 10) - 1;
      const greenChannel = parseInt(fixtureData["channels"]["g"], 10) - 1;
      const blueChannel = parseInt(fixtureData["channels"]["b"], 10) - 1;
      const intensChannel = parseInt(fixtureData["channels"]["i"], 10) - 1;
      const universe = parseInt(this.fixtures[fix_id]["universe"], 10) - 1;
      const channel = parseInt(this.fixtures[fix_id]["channel"], 10) - 1;

      const redVal =
        parseFloat(this.universes[universe][channel + redChannel]) *
        parseFloat(this.universes[universe][channel + intensChannel] / 255);
      const greenVal =
        parseFloat(this.universes[universe][channel + greenChannel]) *
        parseFloat(this.universes[universe][channel + intensChannel] / 255);
      const blueVal =
        parseFloat(this.universes[universe][channel + blueChannel]) *
        parseFloat(this.universes[universe][channel + intensChannel] / 255);
      return `rgb(${redVal}, ${greenVal}, ${blueVal})`;
    },
  },
  props: {
    fixtures: {
      type: Object,
      required: true,
    },
    layout: {
      type: Object,
      required: true,
    },
    cuelists: {
      type: Object,
      required: true,
    },
    universes: {
      type: Object,
      required: true,
    },
  },
};
</script>

<style>
.canvas {
  height: 100%;
  width: 100%;
  font-family: Inter;
  position: relative;
}

.display {
  width: 100%;
  height: 100%;
  border-radius: 25px;
  background-color: white;
}
</style>
