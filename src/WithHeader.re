module Styles = {
  open Css;
  let menu = style([`declaration(("lineHeight", "64px !important"))]);
  let content = style([padding2(~v=0 |> px, ~h=50 |> px)]);
  let childbox = style([background("fff" |> hex), padding(24 |> px), minHeight(280 |> px)]);
  let breadcrumb = style([margin2(~v=16 |> px, ~h=16 |> px) |> important]);
};

let component = ReasonReact.statelessComponent("Header");

let make = children => {
  ...component,
  render: _self =>
    Antd.(
      Layout.(
        ReasonReact.(
          <Layout>
            <Header>
              <Menu
                theme=`Dark mode=`Horizontal defaultSelectedKeys=[|"1"|] className=Styles.menu>
                <Menu.Item key="1">
                  <div onClick=(_e => ReasonReact.Router.push("/") |> ignore)>
                    ("Home" |> string)
                  </div>
                </Menu.Item>
                <Menu.Item key="2">
                  <div onClick=(_e => ReasonReact.Router.push("/about") |> ignore)>
                    ("About" |> string)
                  </div>
                </Menu.Item>
              </Menu>
            </Header>
            <Content className=Styles.content>
              <Breadcrumb className=Styles.breadcrumb>
                <Breadcrumb.Item> ("Home" |> string) </Breadcrumb.Item>
              </Breadcrumb>
              (
                ReasonReact.createDomElement(
                  "div",
                  ~props={"className": Styles.childbox},
                  children,
                )
              )
            </Content>
          </Layout>
        )
      )
    ),
};
