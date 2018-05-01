let component = ReasonReact.statelessComponent("About Page");

module Styles = {
  let title = Css.(style([fontSize(48 |> px)]));
  let container =
    Css.(
      style([
        selector(
          " " ++ title,
          [fontWeight(800), width(100. |> pct), Css.float(`left), width(100. |> pct)],
        ),
      ])
    );
  let infoContainer =
    Css.(
      style([
        padding2(~v=32 |> px, ~h=32 |> px),
        selector(" h2", [fontSize(32 |> px), marginBottom(16 |> px)]),
        selector(" h3", [fontSize(24 |> px), fontWeight(600), marginBottom(16 |> px)]),
        selector(" p", [color(black)]),
        selector(" .box", [width(50. |> pct), height(200 |> px), Css.float(`left)]),
      ])
    );
};

let make = _children => {
  ...component,
  render: _self =>
    ReasonReact.(
      <div className=Styles.container>
        <h1 className=Styles.title> ("About" |> string) </h1>
        <div className=Styles.infoContainer>
          <div className="box">
            <h3> ("Developed By" |> string) </h3>
            <p> ("Nuttapol Laoticharoen" |> string) </p>
            <p> ("#5710500208" |> string) </p>
          </div>
          <br />
          <div className="box">
            <h3> ("Advisor" |> string) </h3>
            <p> ("Paruj Ratanaworabhan" |> string) </p>
          </div>
          <div className="box">
            <h3> ("Implementation" |> string) </h3>
            <p> ("Frontend: ReasonML (Reason-React)" |> string) </p>
            <p> ("Backend: Rust" |> string) </p>
          </div>
        </div>
      </div>
    ),
};
