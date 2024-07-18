<template>
  <div>
  <v-card
    max-width="250"
    height="93.0vh"
    tile
  >
    <v-list dense>
      <v-subheader>SCRIPTS: {{ selectedScript }}</v-subheader>
      <v-list-item-group
        v-model="selectedScript"
        color="primary"
      >
        <v-list-item
          v-for="(script, i) in scripts"
          :key="i"
          @click="getSelectedScript(i)"
        >
          <v-list-item-content>
            <v-list-item-title v-text="script" />
          </v-list-item-content>
        </v-list-item>
      </v-list-item-group>
    </v-list>
  </v-card>
  <v-btn depressed height="7.0vh" @click="updateScripts()">
      Refresh Scripts
  </v-btn>
  </div>
</template>

<script>
export default {
  name: 'ScriptBar',
  props: ['scripts'],
  data () {
    return {
      selectedScript: undefined
    }
  },
  methods: {
    getSelectedScript (value) {
      this.$emit('getScript', value)
    },

    updateScripts () {
      this.$emit('updateScripts')
    }
  }
}
</script>

<style scoped>
.v-btn {
  width: 250px;
  border-radius: 0px;
}
.v-list{
  height:93.0vh;
  overflow-y:auto
}
</style>