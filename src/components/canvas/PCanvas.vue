<template>
  <div class="canvas" @click.self="selectedFixtures = []">
    <div class="fixtures" v-if="fixtures">
      <fixture-item
        v-for="x in Object.keys(fixtures)"
        :key="x"
        :id="x"
        :data="{
          color: getFixtureColorVals(x),
          posX: layout[x].x,
          posY: layout[x].y,
        }"
        :class="selectedFixtures.includes(x) ? 'select' : ''"
        @click="selectedFixtures.push(x)"
      />
    </div>
    <input type="color" @input="handleColorChange" />
  </div>
</template>

<script>
import FixtureItem from "../items/FixtureItem";

export default {
  name: "PCanvas",
  components: {
    FixtureItem,
  },
  data() {
    return {
      selectedFixtures: [],
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
  },
  methods: {
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
