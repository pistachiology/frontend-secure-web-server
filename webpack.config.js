const path = require('path');
const outputDir = path.join(__dirname, 'build/');

const isProd = process.env.NODE_ENV === 'production';
const proxy = require('./config/proxy');

const HtmlWebpackPlugin = require('html-webpack-plugin');
const Dotenv = require('dotenv-webpack');

module.exports = {
  entry: {
    index: './src/index.js'
  },
  mode: isProd ? 'production' : 'development',
  module: {
    rules: [
      {
        test: /\.less$/,
        use: ['style-loader', 'css-loader', 'less-loader']
      }
    ]
  },
  output: {
    path: outputDir,
    publicPath: outputDir,
    filename: '[name].js'
  },
  devServer: {
    contentBase: outputDir,
    compress: true,
    proxy: proxy
  },
  plugins: [
    new HtmlWebpackPlugin({
      filename: 'index.html',
      template: './src/index.html',
      inject: true
    }),
    new Dotenv()
  ]
};
