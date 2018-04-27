open Belt;

let component = ReasonReact.statelessComponent("About Page");

module Styles = {
  let title = Css.(style([fontSize(48 |> px)]));
  let container =
    Css.(style([selector(" " ++ title, [fontWeight(800), width(100. |> pct)])]));
};

let make = _children => {
  ...component,
  render: _self =>
    ReasonReact.(
      <div className=Styles.container> <h1 className=Styles.title> ("About" |> string) </h1> </div>
    ),
};
