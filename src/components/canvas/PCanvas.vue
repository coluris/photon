<template>
  <div
    class="canvas"
    id="canvas"
  >
    <div class="fixtures" v-if="fixtures">
      <fixture-item
        v-for="x in Object.keys(fixtures)"
        :key="x"
        :id="x"
        :data="{
          color: getFixtureColorVals(x),
          posX: this.layout['Layout 1'][x].x,
          posY: this.layout['Layout 1'][x].y,
        }"
        @created="updateFixtureSelectable"
      />
    </div>
  </div>
</template>

<script>
import FixtureItem from "../items/FixtureItem";
import DragSelect from "dragselect";

export default {
  name: "PCanvas",
  components: {
    FixtureItem,
  },
  data() {
    return {
      selectedFixtures: [],
      dragging: false,
      mutLayout: {},
      lastX: 0,
      lastY: 0,
      ds: null,
    };
  },
  mounted() {
    this.mutLayout = this.layout;
     this.ds = new DragSelect({
      selectables: document.querySelectorAll('.fix'),
      area: document.querySelector('#canvas'),
    });
    this.ds.subscribe('callback', (e) => {
      const canvas = document.getElementById('canvas');
      const canvBounds = canvas.getBoundingClientRect();
      this.selectedFixtures.forEach((val) => {
        const fixture = document.getElementById(val);
        const fixBounds = fixture.getBoundingClientRect();
        const leftPercent = (fixBounds.x - canvBounds.left) / canvBounds.width;
        const topPercent = (fixBounds.y - canvBounds.top) / canvBounds.height;
        this.mutLayout["Layout 1"][val].x = leftPercent * 100;
        this.mutLayout["Layout 1"][val].y = topPercent * 100;
            window.ipc.send("layout", {
            layoutName: "Layout 1",
            fixID: val,
            x: this.mutLayout["Layout 1"][val].x,
            y: this.mutLayout["Layout 1"][val].y,
          });
        });
        this.selectedFixtures = e.items.map(fix => fix.id);
    });
  },
  computed: {
    fixListAsString() {
      let output = "";
      for (const fix in this.selectedFixtures) {
        output += `${this.selectedFixtures[fix]},`;
      }
      return output;
    },
  },
  methods: {
    updateFixtureSelectable() {
      this.ds.addSelectables(document.querySelectorAll('.fix'));
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

      let redVal = parseFloat(this.universes[universe][channel + redChannel]);
      let greenVal = parseFloat(
        this.universes[universe][channel + greenChannel]
      );
      let blueVal = parseFloat(this.universes[universe][channel + blueChannel]);

      if (!Number.isNaN(intensChannel)) {
        const intensity = parseFloat(
          this.universes[universe][channel + intensChannel] / 255
        );
        redVal *= intensity;
        greenVal *= intensity;
        blueVal *= intensity;
      }
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

* { user-select: none; }


.canvas {
  height: 100%;
  width: 100%;
  font-family: Inter;
  position: relative;
  background-color: #2f3241;
  border-radius: 10px;
  height: 100%;
  width: 75%;
  overflow: none;
}

.display {
  width: 100%;
  height: 100%;
  border-radius: 25px;
  background-color: white;
}

/* width */
::-webkit-scrollbar {
  width: 5px;
  height: 5px;
}

/* Track */
::-webkit-scrollbar-track {
  width: 5px;
  height: 5px;
  background: #00000000;
}

::-webkit-scrollbar-corner {
  background: #00000000;
}

/* Handle */
::-webkit-scrollbar-thumb {
  background: #555;
  border-radius: 5px;
}

/* Handle on hover */
::-webkit-scrollbar-thumb:hover {
  background: #888;
}
</style>
