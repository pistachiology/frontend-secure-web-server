open Belt;

[%bs.raw {|require("antd/lib/message/style")|}];

/* Dirty binding */
module Message = {
  [@bs.module "antd/lib/message"]
  external info : (~content: string, ~duration: float=?, ~onClose: unit => unit=?, unit) => unit =
    "info";
  [@bs.module "antd/lib/message"]
  external error : (~content: string, ~duration: float=?, ~onClose: unit => unit=?, unit) => unit =
    "error";
};

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
  let memoryAbuseContainer =
    Css.(
      style([
        width(100. |> pct),
        Css.float(`left),
        selector(" span", [fontWeight(800), paddingRight(8 |> px)]),
        selector(" input", [border(1 |> px, `solid, "000" |> hex)]),
        selector(" >*", [marginRight(8 |> px)]),
      ])
    );
};

let convertRlimitToDataSource = (rlimit: Fetcher.RLimitConfig.data) => [
  ("cur_cpu_limit", rlimit.curCpuLimit |> string_of_int),
  ("max_cpu_limit", rlimit.maxCpuLimit |> string_of_int),
  ("cur_as_limit", rlimit.curAsLimit |> string_of_int),
  ("max_as_limit", rlimit.maxAsLimit |> string_of_int),
  ("cur_stack_limit", rlimit.curStackLimit |> string_of_int),
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

module Component = {
  type action =
    | SetMemory(int);
  type state = {memory: int};
  let component = ReasonReact.reducerComponent("HomePageContainer");
  let make = (~rlimit, ~fdList, ~mountList, _children) => {
    ...component,
    initialState: () => {memory: 1024},
    reducer: (action, _state) =>
      switch (action) {
      | SetMemory(value) => ReasonReact.Update({memory: value})
      },
    render: self =>
      ReasonReact.(
        <div className=Styles.container>
          <h1 className=Styles.title> ("Dashboard" |> string) </h1>
          <div className=Styles.infoContainer>
            <h2> ("RLimit" |> string) </h2>
            <SimpleTable dataSource=(rlimit |> convertRlimitToDataSource) />
          </div>
          <div className=Styles.infoContainer>
            <h2> ("FD List" |> string) </h2>
            <h3> (fdList |> listToString |> string) </h3>
          </div>
          <div className=Styles.infoContainer>
            <h2> ("Mount List" |> string) </h2>
            (
              mountList
              |> List.mapWithIndex(_, (ikey, text) =>
                   <h3 key=(ikey |> string_of_int)> (text |> string) </h3>
                 )
              |> List.toArray
              |> array
            )
          </div>
          <div className=Styles.infoContainer>
            <h2> ("Memory Limit" |> string) </h2>
            <div className=Styles.memoryAbuseContainer>
              <span> ("Memory Size (byte): " |> string) </span>
              <input
                _type="number"
                name="memorySize"
                value=(self.state.memory |> string_of_int)
                onChange=(
                  event => {
                    let target =
                      event |> ReactEventRe.Synthetic.target |> ReactDOMRe.domElementToObj;
                    Js.log(target##value);
                    self.send(
                      SetMemory(
                        switch (target##value) {
                        | "" => 0
                        | myMessage =>
                          switch (myMessage |> int_of_string) {
                          | value => value
                          | exception (Failure("int_of_string")) => self.state.memory
                          }
                        },
                      ),
                    );
                  }
                )
              />
              <Antd.Button
                _type=`primary
                onClick=(
                  _event =>
                    Fetcher.Allocate.fetch(~query="/" ++ string_of_int(self.state.memory), ())
                    |> Js.Promise.then_(res => {
                         let _ =
                           switch (res |> Fetch.Response.status) {
                           | 400 => Message.error(~content="container fail to allocate", ())
                           | _ => Message.info(~content="container allocate successfully", ())
                           };
                         Js.Promise.resolve(res);
                       })
                    |> ignore
                )>
                "Allocate"
              </Antd.Button>
            </div>
          </div>
        </div>
      ),
  };
};

let fetchers = (Fetcher.RLimit.c(), Fetcher.FdList.c(), Fetcher.MountList.c());

let component = ReasonReact.statelessComponent("HomePage");

let make = _children => {
  ...component,
  render: _self =>
    ReasonReact.(
      <Fetcher.Composer3
        compose=fetchers
        render=(
          ((rlimit, fdList, mountList)) =>
            switch (rlimit.state.status, fdList.state.status, mountList.state.status) {
            | (Fetcher.Idle, Fetcher.Idle, Fetcher.Idle) =>
              <Component
                rlimit=rlimit.state.data
                fdList=fdList.state.data
                mountList=mountList.state.data
              />
            | _ => <div key="4"> ("Loading" |> string) </div>
            }
        )
      />
    ),
};
