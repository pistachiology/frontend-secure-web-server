/* eslint-disable no-console */
const express = require('express')

process.env.NODE_ENV == 'production' ? require('dotenv').config({ path: '.env.production' }) : require('dotenv').config()
const devProxy = require('./config/proxy')
const proxyMiddleware = require('http-proxy-middleware')

const app = express()

if (devProxy) {
  Object.keys(devProxy).forEach(function(context) {
    console.log(context)
    app.use(proxyMiddleware(context, devProxy[context]))
  })
}
app.use(
  proxyMiddleware({
    target: 'http://localhost:' + process.env.PORT,
    changeOrigin: true,
    onError: function(err, req, res) {
      if (err.code === 'ECONNREFUSED') {
        setTimeout(() => {
          proxyMiddleware({
            target: 'http://localhost:' + process.env.PORT,
            changeOrigin: true
          })(req, res)
        }, 5000)
      }
    }
  })
)

app.listen(process.env.PROXY_PORT, () => {
  console.log(`Proxy is listening on port ${process.env.PROXY_PORT}`)
})
