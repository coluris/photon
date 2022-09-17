<template>
    
    <div class="picker">
        <div 
        class="picker-color"
        @mouseleave="() => dragging=false"
        @mousedown="() => dragging=true"
        @mousemove="movePicker"
        @mouseup="() => dragging=false"
        @click="handleClick"
        :style="`background: linear-gradient(to right, white, hsl(${color * 3.6}deg, 100%, 50%));`">
            
                <div class="picker-selection"
                    :style="`bottom: ${posY}%; left: ${posX-2.5}%;`"
                />
        </div>
        <div class="picker-slider">
            <div class="l-slider">
                <el-slider :style="`--el-color-primary: hsl(${color * 3.6}deg, 100%, 50%);`" v-model="color"/>
            </div>        
        </div>
    </div>
    
</template>

<script>
export default {
        name: 'ColorPicker',
        data(){
            return {
                color: 0,
                posX: 45,
                posY: 50,
                dragging: false,
                element: false,     
            };
        },
        methods: {
            movePicker(e){
                if(this.dragging && e.srcElement.classList.contains('picker-color')){
                    this.posX = (e.clientX / e.srcElement.offsetWidth) * 100;
                    this.posY = 100 - (((e.offsetY) / e.srcElement.offsetHeight) * 100);
                    
                }
            },
            toggleDragging(){
                this.dragging = !this.dragging;
                
            },
            handleClick(e){
                this.posX = (e.clientX / e.srcElement.offsetWidth) * 100;
                this.posY = 100 - (((e.offsetY) / e.srcElement.offsetHeight) * 100);
            },
        }
    }
</script>
    
<style>
.picker-color{
    height: 100%;
    width: 100%;
}
.picker-color::before{
    content: "";
    display: block;
    height: 100%;
    background: linear-gradient(to right, black, black);
  -webkit-mask: linear-gradient(to bottom,#0000, #000);
          mask: linear-gradient(to bottom,#0000, #000);

}
.picker{
    display: flex;
    flex-direction: column;
    height: auto;
    width: auto;
}
.picker-slider{
    display: flex;
    align-items: center;
    justify-content: center;
    width: 100%;
}
.l-slider{
    width: 80%;
}

.picker-selection{
    position: relative;
    height: 5%;
    aspect-ratio : 1 / 1;
    border: solid white 2px;
    mix-blend-mode: difference;

}
.picker-region{
    height: 100%;
    width: 100%;
}

</style>
