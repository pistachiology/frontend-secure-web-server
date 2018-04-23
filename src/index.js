import { AppContainer } from 'react-hot-loader';
import React from 'react';
import ReactDOM from 'react-dom';
const RootComponent = require('./RootComponent.bs').jsComponent;

const render = Component => {
  ReactDOM.render(
    React.createElement(
      AppContainer,
      null,
      React.createElement(Component, null)
    ),
    document.getElementById('root')
  );
};

render(require('./RootComponent.bs').jsComponent);

// Webpack Hot Module Replacement API
if (module.hot) {
  module.hot.accept('./RootComponent.bs', () => {
    render(require('./RootComponent.bs').jsComponent);
  });
}
