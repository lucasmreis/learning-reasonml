let component = ReasonReact.statelessComponent("CardView");

let make = (~code, ~imageSource, ~flipped, ~onClick, _children) => {
  ...component,
  render: _self => {
    let className =
      switch flipped {
      | CardState.Flipped => "Card flipped"
      | CardState.NotFlipped => "Card"
      };
    <div className onClick>
      <div className="Card front">
        <img alt=(ParseAndRenderCard.parseAndRender(code)) src=imageSource />
      </div>
      <div className="Card back" />
    </div>;
  }
};