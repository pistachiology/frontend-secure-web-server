module Styles = {
  open Css;
  let menu = style([`declaration(("lineHeight", "64px !important"))]);
};

let component = ReasonReact.statelessComponent("Header");

let make = children => {
  ...component,
  render: _self => {
    open Antd;
    open Layout;
    let s = ReasonReact.stringToElement;
    <Layout>
      <Header className="header">
        <Menu theme=`Dark mode=`Horizontal defaultSelectedKeys=[|"2"|] className=Styles.menu>
          <Menu.Item key="1"> ("nav 1" |> s) </Menu.Item>
          <Menu.Item key="2"> ("nav 2" |> s) </Menu.Item>
          <Menu.Item key="3"> ("nav 3" |> s) </Menu.Item>
        </Menu>
      </Header>
      <Content> ...children </Content>
    </Layout>;
  },
};
