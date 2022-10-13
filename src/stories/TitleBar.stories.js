import WinTitleBar from "../components/title-bar/WinTitleBar.vue";
import MacTitleBar from "../components/title-bar/MacTitleBar.vue";

export default {
  title: "TitleBar/TitleBar",
  component: WinTitleBar,
  parameters: {
    // More on Story layout: https://storybook.js.org/docs/vue/configure/story-layout
    layout: "fullscreen",
  },
};

const WinTitleBarTemplate = () => ({
  // Components used in your story `template` are defined in the `components` object
  components: { WinTitleBar },

  // Here we define the `template`
  template: "<win-title-bar />",
});

const MacTitleBarTemplate = () => ({
  // Components used in your story `template` are defined in the `components` object
  components: { MacTitleBar },

  // Here we define the `template`
  template: "<mac-title-bar />",
});

export const Windows = WinTitleBarTemplate.bind({});
export const Mac = MacTitleBarTemplate.bind({});
