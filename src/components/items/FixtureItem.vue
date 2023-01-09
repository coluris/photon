<template>
    <div
      :class="'fix'"
      :style="`top: ${posY}%; 
              left: ${posX}%;
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
    emits: ['created'],
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
    data() {
        return {
            posX: 0,
            posY: 0,
        };
    },
    mounted() {
      this.$emit('created');
      this.posX = this.data.posX;
      this.posY = this.data.posY;
    },
    methods: {
      handleDrag(event) {
        console.log(event);
      },
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
    position: relative;
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
  .ds-selected {
    border-color: lightskyblue;
  }
  </style>