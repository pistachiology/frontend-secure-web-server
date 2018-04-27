type route =
  | Home
  | About;

type state = {route};

type action =
  | UpdateRoute(route);

let component = ReasonReact.reducerComponent("RootComponent");

let make = _children => {
  ...component,
  initialState: () => {route: Home},
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          self.send(
            UpdateRoute(
              switch (url.path) {
              | [""] => Home
              | ["index"] => Home
              | ["about"] => About
              | _ => Home
              },
            ),
          )
        ),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  reducer: (action, _state) =>
    switch (action) {
    | UpdateRoute(route) =>
      Js.log2("update route ", route);
      ReasonReact.Update({route: route});
    },
  render: self =>
    <WithHeader>
      (
        switch (self.state.route) {
        | Home => <Home />
        | About => <About />
        }
      )
    </WithHeader>,
};

let jsComponent = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));
