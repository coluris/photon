<template>
    <div class="select-box" :style="`top: ${cssPosition.top}; left: ${cssPosition.left}; height: ${cssPosition.height}; width: ${cssPosition.width};`" />
</template>

<script>
export default {
    name: "SelectionBox",
    computed: {
        cssPosition() {
            const height = Math.abs(this.endY - this.startY);
            const width = Math.abs(this.endX - this.startX);
            const top = Math.min(this.endY, this.startY);
            const left = Math.min(this.endX, this.startX);
            return {
                top: `${top}px`,
                left: `${left}px`,
                height: `${height}px`,
                width: `${width}px`,
            }
        }
    },
    props: {
        startX: {
            type: Number,
            required: true,
        },
        startY: {
            type: Number,
            required: true,
        },
        endX: {
            type: Number,
            required: true,
        },
        endY: {
            type: Number,
            required: true,
        },
    }
};
</script>

<style scoped>
    .select-box{
        border: white dashed 2px;
        position: absolute;
        z-index: 100;
    }
</style>