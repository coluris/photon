<template>
  <div class="dropdown-menu">
    <div class="option-groups" v-for="(option, i) in options" :key="option">
      <div
        class="option-wrapper"
        v-for="sub_option in option"
        :key="sub_option"
      >
        <div class="options">
          {{ sub_option }}
        </div>
      </div>
      <hr v-if="i !== option.length" />
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
};
</script>

<style>
.dropdown-menu {
  display: flex;
  flex-direction: column;
  font-size: 0.6875rem;
  font-family: Inter;
  background-color: #ececec;
  width: fit-content;
  padding-top: 1%;
  padding-bottom: 1%;
  border-radius: 3%;
  box-shadow: 0px 4px 26px 13px rgba(0, 0, 0, 0.1);
}

.options-group {
  display: flex;
  justify-content: left;
  flex-direction: column;
  padding-top: 0.6875rem;
  height: 0.6875rem;
}

.options {
  padding-left: 5px;
  padding-right: 10px;
  padding-top: 0.34375rem;
  padding-bottom: 0.34375rem;
}

.option-wrapper {
  height: max-content;
}

.option-wrapper:hover {
  background-color: #d6d6d6;
  cursor: pointer;
}

hr {
  width: 100%;
}
</style>
