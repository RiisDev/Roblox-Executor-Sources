<template>
  <v-app>
    <div class="main">
      <scriptBar :scripts="scriptNames" @getScript="getScript" @updateScripts="updateScripts" />
      <dataCard v-if="scriptBytecode.length && selectedScript != undefined" :data="toHexString(scriptBytecode)" />
      <dataCard v-if="scriptBytecode.length && selectedScript != undefined" :data="arrayToAscii(scriptBytecode)" />
     </div>
  </v-app>
</template>

<script>
import scriptBar from '@/components/scriptBar.vue'
import dataCard from '@/components/dataCard.vue'
import comms from '@/services/communication.js'

export default {
  name: 'IndexPage',

  components: {
    scriptBar,
    dataCard
  },

  data () {
    return {
      selectedScript: undefined,
      scriptNames: [],
      scriptBytecode: []
    }
  },

  methods: {
    callback(messageObj) {
      if (messageObj.event == "scripts")
        this.scriptNames = messageObj.data;

      else if (messageObj.event == "scriptdata")
        this.scriptBytecode = messageObj.data;
    },

    getScript(value) {
      if (this.selectedScript != value)
      {
        this.selectedScript = value

        comms.sendData(JSON.stringify({ "event": "getscript", "data": value }));
      }
      else
      {
        this.selectedScript = undefined;
      }
    },

    updateScripts() {
      comms.sendData(JSON.stringify({ "event": "updatescripts" }));
    },

    arrayToAscii(arr) {
      let res = [];

      for (let i = 0; i < arr.length; i++)
        res.push(String.fromCharCode(arr[i]));

      return res.join("");
    },

    toHexString(byteArray) {
      return Array.from(byteArray, function(byte) {
        return ('0' + (byte & 0xFF).toString(16)).slice(-2);
      }).join(' ')
   }
  },

  mounted() {
    comms.setCallback(this.callback.bind(this));
  },

  beforeDestroy() {
    comms.setCallback(null);
  }
}
</script>

<style>
.main {
  display: flex;
}
</style>