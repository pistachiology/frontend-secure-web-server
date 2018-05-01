type state = {route: Route.t};

type action =
  | UpdateRoute(ReasonReact.Router.url);

let component = ReasonReact.reducerComponent("RootComponent");

let make = _children => {
  ...component,
  initialState: () => {route: Home},
  didMount: self => {
    self.send(UpdateRoute(ReasonReact.Router.dangerouslyGetInitialUrl()));
    ();
  },
  subscriptions: self => [
    Sub(
      () => ReasonReact.Router.watchUrl(url => self.send(UpdateRoute(url))),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  reducer: (action, _state) =>
    switch (action) {
    | UpdateRoute(url) =>
      let route = Route.handleRoutes(url);
      ReasonReact.Update({route: route});
    },
  render: self =>
    <WithHeader route=self.state.route>
      (Route.handleRouteComponents(self.state.route))
    </WithHeader>,
};

let jsComponent = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));
