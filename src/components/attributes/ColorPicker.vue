<template>
  <div class="color-picker" :style="`background-color: ${bgColor}88;`">
    <color-picker alpha-channel="hide" @color-change="processColorChange" />
  </div>
</template>

<script>
import { ColorPicker } from "vue-accessible-color-picker";
export default {
  name: "PhotonColorPicker",
  components: {
    ColorPicker,
  },
  emits: ["color"],
  data() {
    return {
      bgColor: "#cccccc",
    };
  },
  methods: {
    processColorChange(e) {
      this.$emit("color", e.colors.hex.slice(0, -2));
      this.bgColor = e.colors.hex.slice(0, -2);
    },
  },
};
</script>

<style>
.color-picker {
  width: 35%;
  height: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
  border-radius: 10px;
}

.vacp-color-picker {
  width: 100%;
  height: 100%;
  padding: 0;
  background-color: rgba(0, 0, 0, 0);
  transform: scale(0.95);
}

.vacp-color-space {
  width: 100%;
}
</style>
