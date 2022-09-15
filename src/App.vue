<template>
  <div class="app">
    
    <win-title-bar v-if="platform==='win32'" />
    <mac-title-bar
      v-if="platform==='darwin' && !fullscreen"
    />
    <div class="rest">
      {{`Welcome to Photon for ${platformName}!`}}
    </div>
  </div>
  </template>
  
  
  <script>
  import WinTitleBar from './components/WinTitleBar';
  import MacTitleBar from './components/MacTitleBar';
  export default {
    name: 'App',
    components: {
      WinTitleBar, MacTitleBar
    },
    data(){
      return {
        platform: '',
        fullscreen: false,
      };
    },
    methods: {
    },
    beforeMount(){
      window.ipc.on('platform', (payload) => {
        this.platform = payload})
      window.ipc.on('size', (payload) => {this.fullscreen = payload})
      window.ipc.send('platform')      
    },
    computed: {
      platformName(){
        let pName = '';
        switch(this.platform){
          case('win32'):
            pName = "Windows";
          break;
          case('darwin'):
            pName = "macOS"
        }
        return pName;
      },
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

    user-select: none;
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
  </style>
  