<template>
  <div class="mac-top-bar">
    <div class="mac-drag-title-area">
      <p>Photon — New Show</p>
    </div>
  </div>
</template>

<script>
export default {
  name: "MacTitleBar",

  props: {
    resolution: {
      required: true,
      type: Object,
    },
  },
};
</script>

<style scope>
.mac-top-bar {
  height: 30px;
  width: 100%;
  background-color: #d9d9d9;
  display: flex;
}
.mag-drag-title-area {
  -webkit-app-region: drag;
  display: flex;
  align-items: center;
  justify-content: center;
  width: 100%;
  font-size: 0.8125rem;
  color: #000000;
}
.buffer {
  -webkit-app-region: drag;
  height: 100%;
  width: 225px;
}
</style>
