[%%raw "import './Card.css'"];

let component = ReasonReact.statelessComponent("CardView");

let make = (~code, ~imageSource, ~flipped, ~onClick, _children) => {
  ...component,
  render: _self =>
    <div className=(flipped ? "Card flipped" : "Card") onClick>
      <div className="Card front">
        <img alt=(ParseAndRenderCard.parseAndRender(code)) src=imageSource />
      </div>
      <div className="Card back" />
    </div>
};