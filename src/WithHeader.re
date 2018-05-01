module Styles = {
  open Css;
  let menu = style([`declaration(("lineHeight", "64px !important"))]);
  let content =
    style([
      padding2(~v=0 |> px, ~h=50 |> px),
      width(100. |> pct),
      Css.float(`left),
      minHeight(100. |> vh),
    ]);
  let childbox =
    style([
      background("fff" |> hex),
      padding(24 |> px),
      minHeight(280 |> px),
      width(100. |> pct),
      Css.float(`left),
    ]);
  let breadcrumb = style([margin2(~v=16 |> px, ~h=16 |> px) |> important]);
};

let component = ReasonReact.statelessComponent("Header");

let make = (~route, children) => {
  ...component,
  render: _self =>
    Antd.(
      Layout.(
        ReasonReact.(
          <Layout>
            <Header>
              <Menu
                theme=`Dark
                mode=`Horizontal
                selectedKeys=[|Route.toKey(route)|]
                className=Styles.menu>
                <Menu.Item key=(Route.toKey(Home))>
                  <div onClick=(_e => ReasonReact.Router.push("/") |> ignore)>
                    (Route.toString(Home) |> string)
                  </div>
                </Menu.Item>
                <Menu.Item key=(Route.toKey(About))>
                  <div onClick=(_e => ReasonReact.Router.push("/about") |> ignore)>
                    (Route.toString(About) |> string)
                  </div>
                </Menu.Item>
              </Menu>
            </Header>
            <Content className=Styles.content>
              <Breadcrumb className=Styles.breadcrumb>
                <Breadcrumb.Item> (Route.toString(route) |> string) </Breadcrumb.Item>
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
