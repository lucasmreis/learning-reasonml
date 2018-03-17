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
  | DrawCards(deck)
  | CardsDrawn(deck)
  | DrawCardsFailed
  | DeckFinished(list(card));

type state =
  | CreatingDeck
  | WaitingForUser(deck)
  | DrawingCards(deck)
  | NoMoreCardsToDraw(list(card))
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

let decodeCard = json =>
  Json.Decode.{
    code: json |> field("code", string),
    image: json |> field("image", string)
  };

let decodeDeck = json =>
  Json.Decode.{
    deckId: json |> field("deck_id", string),
    remaining: json |> field("remaining", int),
    cards: json |> field("cards", list(decodeCard))
  };

let component = ReasonReact.reducerComponent("App");

let createDeckSideEffects = self =>
  ReasonReact.(
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
      |> ignore
    )
  );

let drawCardsSideEffects = (stateDeck, self) =>
  ReasonReact.(
    Js.Promise.(
      Fetch.fetch(
        "https://deckofcardsapi.com/api/deck/"
        ++ stateDeck.deckId
        ++ "/draw/?count="
        ++ drawQuantity(stateDeck)
      )
      |> then_(Fetch.Response.json)
      |> then_(json =>
           json
           |> decodeDeck
           |> (
             receivedDeck =>
               if (receivedDeck.remaining > 0) {
                 self.send(
                   CardsDrawn({
                     ...receivedDeck,
                     cards: stateDeck.cards @ receivedDeck.cards
                   })
                 );
               } else {
                 self.send(DeckFinished(stateDeck.cards));
               }
           )
           |> resolve
         )
      |> ignore
    )
  );

let renderCards = cards => {
  let cardElements =
    List.map(c => <CardContainer code=c.code imageSource=c.image />, cards)
    |> Array.of_list
    |> ReasonReact.arrayToElement;
  <div className="App card-list"> cardElements </div>;
};

let renderButtonAndCards = (deck, send, ~disabledButton) =>
  <div className="App">
    <button
      className="App main-action"
      disabled=(Js.Boolean.to_js_boolean(disabledButton))
      onClick=(_self => send(DrawCards(deck)))>
      (ReasonReact.stringToElement("Draw " ++ drawQuantity(deck)))
    </button>
    (renderCards(deck.cards))
  </div>;

let make = _self => {
  ...component,
  initialState: () => CreatingDeck,
  reducer: (action, _state) =>
    switch action {
    | CreateDeck =>
      ReasonReact.UpdateWithSideEffects(CreatingDeck, createDeckSideEffects)
    | DeckCreated(deck) => ReasonReact.Update(WaitingForUser(deck))
    | CreateDeckFailed => ReasonReact.Update(Error)
    | DrawCards(stateDeck) =>
      ReasonReact.UpdateWithSideEffects(
        DrawingCards(stateDeck),
        drawCardsSideEffects(stateDeck)
      )
    | CardsDrawn(deck) => ReasonReact.Update(WaitingForUser(deck))
    | DrawCardsFailed => ReasonReact.Update(Error)
    | DeckFinished(cards) => ReasonReact.Update(NoMoreCardsToDraw(cards))
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
          renderButtonAndCards(deck, self.send, ~disabledButton=false)
        | DrawingCards(deck) =>
          renderButtonAndCards(deck, self.send, ~disabledButton=true)
        | NoMoreCardsToDraw(cards) => renderCards(cards)
        | Error =>
          <p>
            (
              ReasonReact.stringToElement(
                "There was an error. Please refresh and try again!"
              )
            )
          </p>
        }
      )
    </div>
};

let default = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));