<template>
  <div class="dropdown-menu">
    <div class="option-groups" v-for="(option, i) in options" :key="option">
      <div
        class="option-wrapper"
        v-for="(sub_option, j) in option"
        :key="sub_option[0]"
        :ref="`option${i}_${j}`"
        @click="(event) => handleClick(event, sub_option[1])"
      >
        <div class="options">
          {{ sub_option[0] }}
        </div>
      </div>
      <hr v-if="i !== options.length - 1 && options.length > 1" />
    </div>
  </div>
</template>

<script>
export default {
  name: "DropdownMenu",
  props: {
    options: {
      type: Array,
      required: true,
    },
  },
  methods: {
    handleClick(event, callback) {
      this.$emit("closemenu");
      callback();
    },
  },
};
</script>

<style>
.sub-menu {
  position: relative;
  z-index: 10;
}
.dropdown-menu {
  display: flex;
  flex-direction: column;
  font-size: 0.75rem;
  font-family: Inter;
  background-color: #ececec;
  width: fit-content;
  padding-top: 0.5%;
  padding-bottom: 0.5%;
  border-radius: 3%;
  box-shadow: 4px 2px 10px 5px rgba(0, 0, 0, 0.1);
}

.option-groups {
  display: flex;
  flex-direction: column;
  justify-content: left;
}

.options {
  padding-left: 5px;
  padding-right: 50px;
  padding-top: 0.34375rem;
  padding-bottom: 0.34375rem;
}

.option-wrapper {
  height: max-content;
  text-align: left;
}

.option-wrapper:hover {
  background-color: #d6d6d6;
  cursor: pointer;
}

hr {
  width: 95%;
}
</style>
