<template>
  <div class="top-bar">
    <div class="drag-title-area">
      <p>Photon — New Show</p>
    </div>
  </div>
</template>

<script>
export default {
  name: "MacTitleBar",
};
</script>

<style scope>
.top-bar {
  height: 30px;
  width: 100%;
  background-color: #1e2029;
  display: flex;
}
.drag-title-area {
  -webkit-app-region: drag;
  display: flex;
  align-items: center;
  justify-content: center;
  width: 100%;
  font-size: 0.8125rem;
  color: #c2c4c7;
}
.buffer {
  -webkit-app-region: drag;
  height: 100%;
  width: 225px;
}
.close-button {
  height: 100%;
  width: 75px;
  border: none;
  background-color: inherit;
  transition: 0.2s ease;
  display: flex;
  justify-content: center;
  align-items: center;
}
.close-button:hover {
  background-color: red;
}
.close-button:active {
  background-color: rgb(161, 0, 0);
}

.menu-button {
  height: 100%;
  width: 75px;
  border: none;
  background-color: inherit;
  transition: 0.2s ease;
  display: flex;
  justify-content: center;
  align-items: center;
}
.menu-button:hover {
  background-color: #363a4b;
}
.menu-button:active {
  background-color: #484d65;
}
.rest {
  font-family: Arial;
  font-size: 3rem;
  color: white;
}
</style>
