<template>
  <div
    class="fix"
    :style="`top: ${posY}%; 
            left: ${posX}%;
            height: ${3 * scale}vh;
            width: ${3 * scale}vh;
            `"
  >
    <div class="head" />
  </div>
</template>

<script>
export default {
  name: "FixtureItem",
  data() {
    return {
      posX: 2,
      posY: 0,
    };
  },
  methods: {},
  props: {
    scale: {
      type: Number,
      default: 1,
    },
  },
};
</script>

<style>
.fixture {
  height: 100%;
  width: 100%;
  border-style: solid;
  border-width: 0.5vh;
  border-color: #47474e;
  background-color: rgb(108, 108, 108);
  position: relative;
  display: flex;
  justify-content: center;
  align-items: center;
}

.head {
  height: 75%;
  width: 75%;
  border-radius: 50%;
  background-color: greenyellow;
  border: solid 0.1vh black;
  position: relative;
}

.select {
  border-color: lightskyblue;
}
</style>
