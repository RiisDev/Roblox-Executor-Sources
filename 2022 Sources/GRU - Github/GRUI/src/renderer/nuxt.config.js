/**
 * By default, Nuxt.js is configured to cover most use cases.
 * This default configuration can be overwritten in this file
 * @link {https://nuxtjs.org/guide/configuration/}
 */

module.exports = {
  ssr: false,
  target: 'static',
  head: {
    title: 'grui',
    meta: [{ charset: 'utf-8' }]
  },
  loading: false,
  plugins: [
    { ssr: true, src: '@/plugins/icons.js' }
  ],
  buildModules: [

  ],
  modules: [
    '@nuxtjs/vuetify'
  ],
  vuetify: {
    theme: {
      themes: {
        light: {
          primary: '#1867c0',
          secondary: '#b0bec5',
          accent: '#8c9eff',
          error: '#b71c1c'
        },
        dark: {
          primary: '#90CAF9',
          background: '#3F51B5'
        }
      }
    }
  }
}
