<template>
  <div>
    <nuxt />
  </div>
</template>

<script>
export default {
  name: 'DefaultLayout',
  components: {},
  created () { this.$vuetify.theme.dark = true }
}
</script>

<style>
body {
  margin: 0 !important;
}
</style>
