open Belt;

let component = ReasonReact.statelessComponent("Home Page");

module Styles = {
  let title = Css.(style([fontSize(48 |> px)]));
  let container =
    Css.(style([selector(" " ++ title, [fontWeight(800), width(100. |> pct)])]));
  let infoContainer =
    Css.(
      style([
        padding2(~v=32 |> px, ~h=32 |> px),
        selector(" h2", [fontSize(32 |> px), marginBottom(16 |> px)]),
        selector(" h3", [fontSize(16 |> px), fontWeight(400), marginLeft(30 |> px)]),
      ])
    );
};

let convertRlimitToDataSource = (rlimit: Fetcher.RLimitConfig.data) => [
  ("cur_cpu_limit", rlimit.curCpuLimit |> string_of_int),
  ("max_cpu_limit", rlimit.maxCpuLimit |> string_of_int),
  ("cur_as_limit", rlimit.curAsLimit |> string_of_int),
  ("max_as_limit", rlimit.maxAsLimit |> string_of_int),
  ("cur_stak_limit", rlimit.curStackLimit |> string_of_int),
  ("max_stack_limit", rlimit.maxStackLimit |> string_of_int),
  ("cur_data_limit", rlimit.curDataLimit |> string_of_int),
  ("max_data_limit", rlimit.maxDataLimit |> string_of_int),
];

let rec listToString_ = d =>
  switch (d) {
  | [last] => last
  | [head, ...tail] => head ++ ", " ++ (tail |> listToString_)
  | _ => ""
  };

let listToString = d => "[" ++ listToString_(d) ++ "]";

let t1 = () => <Fetcher.RLimit render=(_state => <div />) />;

let t2 = () => <Fetcher.MountList render=(_state => <div />) />;

let test = t => t ? t2() : t1();

let make = _children => {
  ...component,
  render: _self =>
    <WithHeader>
      <Fetcher.RLimit
        render=(
          ({state: rlimitState}) =>
            <Fetcher.MountList
              render=(
                ({state: mountListState}) =>
                  <Fetcher.FdList
                    render=ReasonReact.(
                             ({state: fdListState}) =>
                               switch (
                                 rlimitState.status,
                                 fdListState.status,
                                 mountListState.status,
                               ) {
                               | (Fetcher.Idle, Fetcher.Idle, Fetcher.Idle) =>
                                 <div className=Styles.container>
                                   <h1 className=Styles.title> ("Dashboard" |> string) </h1>
                                   <div className=Styles.infoContainer>
                                     <h2> ("RLimit" |> string) </h2>
                                     <SimpleTable
                                       dataSource=(rlimitState.data |> convertRlimitToDataSource)
                                     />
                                   </div>
                                   <div className=Styles.infoContainer>
                                     <h2> ("FD List" |> string) </h2>
                                     <h3> (fdListState.data |> listToString |> string) </h3>
                                   </div>
                                   <div className=Styles.infoContainer>
                                     <h2> ("Mount List" |> string) </h2>
                                     (
                                       mountListState.data
                                       |> List.mapWithIndex(_, (ikey, text) =>
                                            <h3 key=(ikey |> string_of_int)> (text |> string) </h3>
                                          )
                                       |> List.toArray
                                       |> array
                                     )
                                   </div>
                                 </div>
                               | _ => <div key="1"> ("Loading" |> string) </div>
                               }
                           )
                  />
              )
            />
        )
      />
    </WithHeader>,
};
