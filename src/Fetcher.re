open Belt;

module type Config = {
  type data;
  type payload;
  /* type query; */
  let name: string;
  let url: string;
  let headers: list((string, string));
  let defaultValue: data;
  let method_: Fetch.requestMethod;
  let encodePayload: payload => Js.Json.t;
  let decodeState: Js.Json.t => data;
};

module type FactoryConfig = {let baseUrl: string;};

type props('a, 'b) = {
  state: 'a,
  fetch: 'b,
};

type status =
  | Idle
  | Fetching
  | Failed;

type abstractData('a) =
  | Success('a)
  | Failed(string)
  | None;

type abstractAction('a) =
  | Fetch
  | ParseData('a)
  | Resolve;

module CreateFetcherFactory = (FC: FactoryConfig) => {
  /* TODO: Add query to fetcher  */
  module CreateFetcher = (C: Config) => {
    type payload = C.payload;
    type state = {
      data: C.data,
      reason: option(string),
      status,
    };
    type data = abstractData(C.data);
    type action = abstractAction(data);
    let url = FC.baseUrl ++ C.url;
    /* type query = C.query; */
    external dictToObject : Js.Dict.t(string) => Js.t({..}) = "%identity";
    /* function */
    let fetch = (~payload=?, ~headers: option(list((string, string)))=?, ()) => {
      let body =
        switch (payload) {
        | Some(payload) =>
          Some(Fetch.BodyInit.make(payload |> C.encodePayload |> Json.stringify))
        | None => None
        };
      let headers =
        Fetch.HeadersInit.make(
          (
            switch (headers) {
            | Some(headers) => headers
            | None => []
            }
          )
          |> Js.Dict.fromList
          |> dictToObject
          |> Js.Obj.assign(C.headers |> Js.Dict.fromList |> dictToObject)
          |> Js.Obj.assign({"Content-Type": "application/json"}),
        );
      Fetch.fetchWithInit(url, Fetch.RequestInit.make(~method_=C.method_, ~body?, ~headers, ()));
    };
    /* module Fetcher = { */
    let component = ReasonReact.reducerComponent(C.name);
    let make = (~render, ~payload=?, ~headers=?, ~lazy_=false, _children) => {
      ...component,
      initialState: () => {data: C.defaultValue, status: Idle, reason: None},
      reducer: (action, state) =>
        switch (action) {
        | Fetch =>
          ReasonReact.UpdateWithSideEffects(
            {...state, status: Fetching},
            (
              self =>
                Js.Promise.(
                  fetch(~payload?, ~headers?, ())
                  |> then_(Fetch.Response.json)
                  |> then_(json => {
                       ParseData(Success(json |> C.decodeState)) |> self.send;
                       resolve(Js.Json.null);
                     })
                  |> catch(err => {
                       Js.log2("Error: ", err);
                       ParseData(Failed("Fetch Error")) |> self.send;
                       resolve(Js.Json.null);
                     })
                  |> ignore
                )
            ),
          )
        | ParseData(fetchedData) =>
          switch (fetchedData) {
          | Success(data) => ReasonReact.Update({status: Idle, reason: None, data})
          | Failed(reason) =>
            ReasonReact.Update({...state, status: Failed, reason: Some(reason)})
          | None => ReasonReact.NoUpdate
          }
        | Resolve => ReasonReact.Update({...state, status: Idle})
        },
      didMount: self => {
        if (lazy_ == false) {
          self.send(Fetch);
        };
        ();
      },
      willReceiveProps: self => {
        self.send(Fetch);
        self.state;
      },
      render: self => {
        let state = self.state;
        let fetch = () => self.send(Fetch);
        let child = {state, fetch};
        render(child);
      },
    };
    let compose = (~payload=?, ~headers=?, ~key=?, ~ref=?, ~lazy_=false, ~render) =>
      ReasonReact.element(~key?, ~ref?, make(~render, ~payload?, ~headers?, ~lazy_, [||]));
  };
  /* }; */
};

include
  CreateFetcherFactory(
    {
      let baseUrl = "/api/";
    },
  );

module Composer = {
  let generate = make => make();
  let component = ReasonReact.statelessComponent("FetcherComposer");
  let make = (~compose, ~render, _children) => {
    ...component,
    render: _self => render(compose |> List.map(_, comp => comp |> generate)),
  };
};

module RLimitConfig = {
  type data = {
    curCpuLimit: int,
    maxCpuLimit: int,
    curAsLimit: int,
    maxAsLimit: int,
    curStackLimit: int,
    maxStackLimit: int,
    curDataLimit: int,
    maxDataLimit: int,
  };
  type payload;
  /* type query; */
  let name = "RLimitFetcher";
  let url = "rlimit";
  let headers = [];
  let defaultValue = {
    curCpuLimit: 0,
    maxCpuLimit: 0,
    curAsLimit: 0,
    maxAsLimit: 0,
    curStackLimit: 0,
    maxStackLimit: 0,
    curDataLimit: 0,
    maxDataLimit: 0,
  };
  let method_ = Fetch.Get;
  let encodePayload = _payload => Js.Json.null;
  let decodeState = json =>
    Json.Decode.{
      curCpuLimit: json |> field("cur_cpu_limit", int),
      maxCpuLimit: json |> field("max_cpu_limit", int),
      curAsLimit: json |> field("cur_as_limit", int),
      maxAsLimit: json |> field("max_as_limit", int),
      curStackLimit: json |> field("cur_stack_limit", int),
      maxStackLimit: json |> field("max_stack_limit", int),
      curDataLimit: json |> field("cur_data_limit", int),
      maxDataLimit: json |> field("max_data_limit", int),
    };
};

module RLimit = CreateFetcher(RLimitConfig);

module FdListConfig = {
  type data = list(string);
  type payload;
  /* type query; */
  let name = "RLimitFetcher";
  let url = "fd";
  let headers = [];
  let defaultValue = [];
  let method_ = Fetch.Get;
  let encodePayload = _payload => Js.Json.null;
  let decodeState = json =>
    switch (Js.Json.decodeArray(json)) {
    | Some(json) =>
      json
      |> List.fromArray
      |> List.keepMap(_, d => d |> Js.Json.decodeNumber)
      |> List.map(_, d => d |> int_of_float |> string_of_int)
    | None => []
    };
};

module FdList = CreateFetcher(FdListConfig);

module MountListConfig = {
  type data = list(string);
  type payload;
  /* type query; */
  let name = "MountList";
  let url = "mount";
  let headers = [];
  let defaultValue = [];
  let method_ = Fetch.Get;
  let encodePayload = _payload => Js.Json.null;
  let decodeState = json =>
    switch (Js.Json.decodeArray(json)) {
    | Some(json) => json |> List.fromArray |> List.keepMap(_, d => d |> Js.Json.decodeString)
    | None => []
    };
};

module MountList = CreateFetcher(MountListConfig);
