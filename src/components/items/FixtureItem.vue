<template>
  <div
    :class="'fix'"
    :style="`top: ${data.posY}vh; 
            left: ${data.posX}vh;
            height: ${3 * scale}vh;
            width: ${3 * scale}vh;
            `"
  >
    <div class="head" :style="`background-color: ${data.color}`" />
  </div>
</template>

<script>
export default {
  name: "FixtureItem",
  props: {
    scale: {
      type: Number,
      default: 1,
    },
    data: {
      type: Object,
      required: true,
    },
  },
  methods: {
  },
};
</script>

<style>
.fix {
  height: 100%;
  width: 100%;
  border-style: solid;
  border-width: 0.5vh;
  border-color: #47474e;
  background-color: rgb(108, 108, 108);
  position: absolute;
  display: flex;
  justify-content: center;
  align-items: center;
  border-radius: 1vh;
}

.head {
  height: 75%;
  width: 75%;
  border-radius: 50%;
  border: solid 0.1vh black;
  position: relative;
}

.select {
  border-color: lightskyblue;
}
</style>
