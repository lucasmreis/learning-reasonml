type action =
  | Flip;

type state = {flipped: bool};

let component = ReasonReact.reducerComponent("CardContainer");

let make = (~code, ~imageSource, _self) => {
  ...component,
  initialState: () => {flipped: true},
  reducer: (_action, state) => ReasonReact.Update({flipped: ! state.flipped}),
  render: self =>
    <CardView
      code
      imageSource
      flipped=self.state.flipped
      onClick=(_event => self.send(Flip))
    />
};