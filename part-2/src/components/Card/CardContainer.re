type action =
  | Flip;

let component = ReasonReact.reducerComponent("CardContainer");

let make = (~code, ~imageSource, _self) => {
  ...component,
  initialState: () => CardState.Flipped,
  reducer: (action, state) =>
    switch action {
    | Flip => ReasonReact.Update(CardState.flip(state))
    },
  render: self => {
    let flipped = self.state;
    <CardView code imageSource flipped onClick=(_event => self.send(Flip)) />;
  }
};

let default =
  ReasonReact.wrapReasonForJs(~component, jsProps =>
    make(~code=jsProps##code, ~imageSource=jsProps##imageSource, [||])
  );