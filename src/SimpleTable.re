let component = ReasonReact.statelessComponent("SimpleTable");

module Styles = {
  let darkColor = Css.hex("f2f2f2");
  let infoTable =
    Css.(
      style([
        display(`flex),
        flexDirection(`column),
        flexWrap(`nowrap),
        border(1 |> px, `solid, darkColor),
        width(100. |> pct),
        selector(" .header", [fontWeight(800), backgroundColor(darkColor)]),
        selector(
          " .row",
          [
            display(`flex),
            width(100. |> pct),
            alignContent(`center),
            padding(8 |> px),
            selector(" .item", [width(50. |> pct), textAlign(`center)]),
          ],
        ),
      ])
    );
};

let make = (~dataSource: 'a, _children) => {
  ...component,
  render: _self =>
    ReasonReact.(
      <div className=Styles.infoTable>
        <div className="header row">
          <div className="item"> ("Key" |> string) </div>
          <div className="item"> ("Value" |> string) </div>
        </div>
        (
          dataSource
          |> List.mapi((key, (propName, value)) =>
               <div className="row" key=(key |> string_of_int)>
                 <div className="item"> (propName |> string) </div>
                 <div className="item"> (value |> string) </div>
               </div>
             )
          |> Array.of_list
          |> array
        )
      </div>
    ),
};
