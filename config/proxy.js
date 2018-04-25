var dotenv = require('dotenv').config();

module.exports = {
  '/api': {
    target: process.env.API_GATEWAY,
    changeOrigin: true,
    logLevel: 'debug'
  }
};
