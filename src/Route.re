type t =
  | Home
  | About;

let handleRoutes = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | [""] => Home
  | ["index"] => Home
  | ["about"] => About
  | _ => Home
  };

let handleRouteComponents = route =>
  switch (route) {
  | Home => <Home />
  | About => <About />
  };

let toKey = route =>
  switch (route) {
  | Home => "1"
  | About => "2"
  };

let toString = route =>
  switch (route) {
  | Home => "Home"
  | About => "About"
  };
