<template>
    <v-card width="100%">
        <v-card class="scroll">
          {{data}}
        </v-card>
     </v-card>
</template>

<script>
export default {
  name: 'dataCard',
  props: ['data'],
  data () {
    return {
    }
  },
  methods: {
  }
}
</script>

<style scoped>
.v-card{
    height:100.0vh;
    overflow-y:auto;
    -ms-overflow-style: none;
    scrollbar-width: none;
}
.v-card::-webkit-scrollbar { 
    display: none;
}
</style>