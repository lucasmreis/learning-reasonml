[%%raw "import './App.css'"];

type card = {
  image: string,
  code: string
};

type deck = {
  deckId: string,
  remaining: int,
  cards: list(card)
};

type action =
  | CreateDeck
  | DeckCreated(deck)
  | CreateDeckFailed
  | Draw;

type state =
  | CreatingDeck
  | WaitingForUser(deck)
  | DrawingCards(deck)
  | Finished(list(card))
  | Error;

let number_of_cards_per_draw = 3;

let drawQuantity = deck =>
  Pervasives.min(deck.remaining, number_of_cards_per_draw) |> Js.Int.toString;

let decodeCreatedDeck = json =>
  Json.Decode.{
    deckId: json |> field("deck_id", string),
    remaining: json |> field("remaining", int),
    cards: []
  };

let component = ReasonReact.reducerComponent("App");

let make = _self => {
  ...component,
  initialState: () => CreatingDeck,
  reducer: (action, _state) =>
    switch action {
    | CreateDeck =>
      ReasonReact.UpdateWithSideEffects(
        CreatingDeck,
        (
          self =>
            Js.Promise.(
              Fetch.fetch("https://deckofcardsapi.com/api/deck/new/shuffle/")
              |> then_(Fetch.Response.json)
              |> then_(json =>
                   json
                   |> decodeCreatedDeck
                   |> (deck => self.send(DeckCreated(deck)))
                   |> resolve
                 )
              |> catch(_error => self.send(CreateDeckFailed) |> resolve)
            )
            |> ignore
        )
      )
    | DeckCreated(deck) => ReasonReact.Update(WaitingForUser(deck))
    | CreateDeckFailed => ReasonReact.Update(Error)
    | Draw => ReasonReact.Update(Error)
    },
  didMount: self => {
    self.send(CreateDeck);
    ReasonReact.NoUpdate;
  },
  render: self =>
    <div className="App">
      (
        switch self.state {
        | CreatingDeck => <p> (ReasonReact.stringToElement("Loading...")) </p>
        | WaitingForUser(deck) =>
          <button
            className="App main-action" onClick=(_self => self.send(Draw))>
            (ReasonReact.stringToElement("Draw " ++ drawQuantity(deck)))
          </button>
        | DrawingCards(_deck) =>
          <p> (ReasonReact.stringToElement("DrawingCards")) </p>
        | Finished(_cards) =>
          <p> (ReasonReact.stringToElement("Finished")) </p>
        | Error => <p> (ReasonReact.stringToElement("Error")) </p>
        }
      )
    </div>
};

let default = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));