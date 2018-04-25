/* This is the basic component. */
let component = ReasonReact.statelessComponent("Home Page");

/* Your familiar handleClick from ReactJS. This mandatorily takes the payload,
   then the `self` record, which contains state (none here), `handle`, `reduce`
   and other utilities */
let handleClick = ((state, _event), _self) => Js.log2("clicked!", state);

/* `make` is the function that mandatorily takes `children` (if you want to use
   `JSX). `message` is a named argument, which simulates ReactJS props. Usage:

   `<Page message="hello" />`

   Which desugars to

   `ReasonReact.element(Page.make(~message="hello", [||]))` */
let s = ReasonReact.stringToElement;

let make = (~message, _children) => {
  ...component,
  render: self =>
    <WithHeader>
      <Fetcher.RLimit
        render=(
          (~state: Fetcher.RLimit.state) =>
            state.status == Fetcher.RLimit.Fetching ?
              <div key="1"> ("Loading" |> s) </div> :
              <div onClick=(e => (state, e) |> self.handle(handleClick)) key="2">
                (message |> s)
                (state.data.curAsLimit |> Js.Int.toString |> s)
              </div>
        )
      />
    </WithHeader>,
};
