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
          posX: mutLayout['Layout 1'][x].x,
          posY: mutLayout['Layout 1'][x].y,
        }"
        :class="getClassname(x)"
        @mousedown="
          (e) => {
            if (!selectedFixtures.includes(x)) {
              selectedFixtures.push(x);
            }
            beginDrag(e);
          }
        "
      />
      <selection-box
        v-if="isMultiSelectEnabled"
        :start-x="selectStart.x"
        :start-y="selectStart.y"
        :end-x="selectEnd.x"
        :end-y="selectEnd.y"
      />
    </div>
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
      dragging: false,
      mutLayout: {},
      lastX: 0,
      lastY: 0,
    };
  },
  mounted() {
    this.mutLayout = this.layout;
    console.log(this.mutLayout);
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
    beginDrag() {
      this.dragging = true;
      const canvas = document.getElementById("canvas");
      const x = event.clientX - canvas.offsetLeft;
      const y = event.clientY - canvas.offsetTop;
      const scaledCoords = this.getScaledCoords(x, y);
      this.lastX = scaledCoords.x;
      this.lastY = scaledCoords.y;
    },
    isSelecting(fix) {
      const fixTop = fix.offsetTop;
      const fixLeft = fix.offsetLeft;
      const fixBottom = fixTop + fix.offsetHeight;
      const fixRight = fixLeft + fix.offsetWidth;
      const selTop = this.selectionBox.top;
      const selLeft = this.selectionBox.left;
      const selBottom = this.selectionBox.bottom;
      const selRight = this.selectionBox.right;

      return (
        (fixTop >= selTop &&
          fixLeft >= selLeft &&
          fixBottom <= selBottom &&
          fixRight <= selRight) || // completely inside
        (fixBottom >= selTop &&
          fixTop < selTop &&
          fixRight > selLeft &&
          fixLeft < selRight) || // partially above
        (fixTop <= selBottom &&
          fixBottom > selBottom &&
          fixRight > selLeft &&
          fixLeft < selRight) || // partially below
        (fixRight >= selLeft &&
          fixLeft < selLeft &&
          fixBottom > selTop &&
          fixTop < selBottom) || // partially to the left
        (fixLeft <= selRight &&
          fixRight > selRight &&
          fixBottom > selTop &&
          fixTop < selBottom) || // partially to the right
        (fixTop < selTop &&
          fixBottom > selBottom &&
          fixLeft < selLeft &&
          fixRight > selRight) || // spans entire width, but top and bottom are above and below
        (fixLeft < selLeft &&
          fixRight > selRight &&
          fixTop < selTop &&
          fixBottom > selBottom) // spans entire height, but left and right are left and right of selection box
      );
    },

    getScaledCoords(x = 0, y = 0) {
      const SCALE_Y_RATIO = 99.5 / document.documentElement.clientHeight;
      const SCALE_X_RATIO = 122.6 / document.documentElement.clientWidth;
      const scalePosX = x * SCALE_X_RATIO;
      const scalePosY = y * SCALE_Y_RATIO;
      return { x: scalePosX, y: scalePosY };
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
      this.dragging = false;
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
      if (this.dragging) {
        const canvas = document.getElementById("canvas");
        const x = event.clientX - canvas.offsetLeft;
        const y = event.clientY - canvas.offsetTop;
        const scaledCoords = this.getScaledCoords(x, y);
        const xDiff = scaledCoords.x - this.lastX;
        const yDiff = scaledCoords.y - this.lastY;
        this.selectedFixtures.forEach((val) => {
          this.mutLayout["Layout 1"][val].x += xDiff;
          this.mutLayout["Layout 1"][val].y += yDiff;

          window.ipc.send("layout", {
            layoutName: "Layout 1",
            fixID: val,
            x: this.mutLayout["Layout 1"][val].x,
            y: this.mutLayout["Layout 1"][val].y,
          });
        });
        this.lastX = scaledCoords.x;
        this.lastY = scaledCoords.y;
      } else if (this.isMultiSelectEnabled) {
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
    handleColorChange() {
      const red = parseInt(this.selectedColor.substring(1, 3), 16);
      const green = parseInt(this.selectedColor.substring(3, 5), 16);
      const blue = parseInt(this.selectedColor.substring(5, 7), 16);

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
    selectedColor: {
      type: String,
      default: "#FFFFFF",
    },
  },
  watch: {
    selectedColor() {
      this.handleColorChange();
    },
    layout() {
      this.mutLayout = this.layout;
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
  background-color: #2f3241;
  border-radius: 10px;
  height: 100%;
  width: 75%;
}

.display {
  width: 100%;
  height: 100%;
  border-radius: 25px;
  background-color: white;
}
</style>
