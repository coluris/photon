import DropdownMenu from "../components/dropdown/DropdownMenu.vue";

export default {
  title: "Dropdown/DropdownMenu",
  component: DropdownMenu,
  parameters: {
    // More on Story layout: https://storybook.js.org/docs/vue/configure/story-layout
    layout: "fullscreen",
  },
};

const DropdownMenuTemplate = () => ({
  // Components used in your story `template` are defined in the `components` object
  components: { DropdownMenu },

  // Here we define the `template`
  template:
    "<dropdown-menu :options=\"[['Option 137218378912', 'Option 2'], ['Section 2 Option 1', 'Sec 2 Option 2']]\" />",
});

export const Menu = DropdownMenuTemplate.bind({});
