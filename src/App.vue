<template>
  <div class="app">
    
    <div class="top-bar">
      <div class="buffer" />
      <div class="drag-title-area">
      <p>Photon — New Show</p>
      </div>
      <button
        class="menu-button" 
        @click="minimize()"
      >
      <svg width="15" height="15" xmlns="http://www.w3.org/2000/svg">

        <line stroke-linecap="undefined" stroke-linejoin="undefined" y2="9" x2="2" y1="9" x1="16" stroke="#C2C4C7" fill="none"/>
      </svg>
      </button>
      <button
        class="menu-button" 
        @click="toggleFullscreen()"
      >
      <svg v-if="!this.fullscreen" width="30" height="30" xmlns="http://www.w3.org/2000/svg">
        <rect height="11" width="11" y="10" x="10" stroke="#C2C4C7" fill="none"/>
      </svg>

      <svg v-if="this.fullscreen" width="30" height="30" xmlns="http://www.w3.org/2000/svg">
        
        <rect height="9" width="9" y="9" x="12" stroke="#C2C4C7" fill="none"/>
        <rect height="9" width="9" y="12" x="9" stroke="#C2C4C7" fill="#1e2029"/>
      </svg>
      </button>
      <button
      class="close-button" 
      @click="closeApp()"
      >
      <svg fill="#C2C4C7" xmlns="http://www.w3.org/2000/svg"  viewBox="0 0 32 32" width="18px" height="18px"><path d="M 7.21875 5.78125 L 5.78125 7.21875 L 14.5625 16 L 5.78125 24.78125 L 7.21875 26.21875 L 16 17.4375 L 24.78125 26.21875 L 26.21875 24.78125 L 17.4375 16 L 26.21875 7.21875 L 24.78125 5.78125 L 16 14.5625 Z"/></svg>
    </button>
    </div>
    <div class="rest">
      Welcome to Photon!
    </div>
  </div>
  </template>
  
  
  <script>
  export default {
    
    name: 'App',
    methods: {
      closeApp(){
        window.ipc.send('menu', 'close')
      },
      toggleFullscreen(){
        if(!this.fullscreen){
          window.ipc.send('menu', 'fullscreenOn')
        }
        else{
          window.ipc.send('menu', 'fullscreenOff')
        }
        this.fullscreen = !this.fullscreen
      },
      minimize(){
        window.ipc.send('menu', 'minimize')
      }
    },
    data(){
      return{
      fullscreen: false,
      };
    },

  }
  </script>
  
  <style>
  .app {
    font-family: Consolas, Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    background-color: #2f3241;
    margin: 0px;
    padding: 0px;
    width: 100%;
    height: 100%;
  }
  body{
    margin: 0;
    overflow-y: hidden;
    overflow-x: hidden;
    background-color: #2f3241;
  }
  .rest{
    margin: 0;
    width: 100%;
    height: 100%;
  }
  .top-bar{
    height: 30px;
    width: 100%;
    background-color: #1e2029;
    display: flex;
  }
  .drag-title-area{
    -webkit-app-region: drag;
    display: flex;
    align-items: center;
    justify-content: center;
    width: 100%;
    font-size: .8125rem;
    color: #C2C4C7;
  }
  .buffer{
    
    -webkit-app-region: drag;
    height: 100%;
    width: 225px;
  }
  .close-button{
    height: 100%;
    width: 75px;
    border: none;
    background-color: inherit;
    transition: .2s ease;
    display: flex;
    justify-content: center;
    align-items: center;
  }
  .close-button:hover{
    background-color: red;
  }
  .close-button:active{
    background-color: rgb(161, 0, 0);
  }
  
  .menu-button{
    height: 100%;
    width: 75px;
    border: none;
    background-color: inherit;
    transition: .2s ease;
    display: flex;
    justify-content: center;
    align-items: center;
  }
  .menu-button:hover{
    background-color: #363a4b;
  }
  .menu-button:active{
    background-color: #484d65;
  }
  .rest{
    font-family: Arial;
    font-size: 3rem;
    color: white;
  }
  </style>
  