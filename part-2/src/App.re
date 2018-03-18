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

let decodeCreatedDeck = json =>
  Json.Decode.{
    deckId: json |> field("deck_id", string),
    remaining: json |> field("remaining", int),
    cards: []
  };

let createDeckSideEffects = self =>
  ReasonReact.(
    Js.Promise.(
      Fetch.fetch("https://deckofcardsapi.com/api/deck/new/shuffle/")
      |> then_(Fetch.Response.json)
      |> then_(json => decodeCreatedDeck(json) |> resolve)
      |> then_(deck => DeckCreated(deck) |> self.send |> resolve)
      |> catch(_error => self.send(CreateDeckFailed) |> resolve)
      |> ignore
    )
  );

let decodeCard = json =>
  Json.Decode.{
    code: json |> field("code", string),
    image: json |> field("image", string)
  };

/* the decodeCard decoder can be used in another
   decoder to deal with nested objects: */
let decodeDeck = json =>
  Json.Decode.{
    deckId: json |> field("deck_id", string),
    remaining: json |> field("remaining", int),
    cards: json |> field("cards", list(decodeCard))
  };

/* helper function to decide whether we should dispatch
   a CardsDrawn or a DeckFinished action: */
let drawnOrFinished = (current, received) =>
  if (received.remaining > 0) {
    CardsDrawn({...received, cards: current.cards @ received.cards});
  } else {
    DeckFinished(current.cards);
  };

/* the Pervasives module contains the min function. If we
   have less than 3 remaining cards, only draw the
   remaining quantity from the API:   */
let number_of_cards_per_draw = 3;

let drawQuantity = deck =>
  Pervasives.min(deck.remaining, number_of_cards_per_draw) |> Js.Int.toString;

let drawCardsSideEffects = (currentDeck, self) =>
  ReasonReact.(
    Js.Promise.(
      Fetch.fetch(
        "https://deckofcardsapi.com/api/deck/"
        ++ currentDeck.deckId
        ++ "/draw/?count="
        ++ drawQuantity(currentDeck)
      )
      |> then_(Fetch.Response.json)
      |> then_(json => decodeDeck(json) |> resolve)
      |> then_(receivedDeck =>
           drawnOrFinished(currentDeck, receivedDeck) |> self.send |> resolve
         )
      |> ignore
    )
  );

let reducer = (action, _state) =>
  switch action {
  | CreateDeck =>
    ReasonReact.UpdateWithSideEffects(CreatingDeck, createDeckSideEffects)
  | DeckCreated(deck) => ReasonReact.Update(WaitingForUser(deck))
  | CreateDeckFailed => ReasonReact.Update(Error)
  | DrawCards(currentDeck) =>
    ReasonReact.UpdateWithSideEffects(
      DrawingCards(currentDeck),
      drawCardsSideEffects(currentDeck)
    )
  | CardsDrawn(deck) => ReasonReact.Update(WaitingForUser(deck))
  | DrawCardsFailed => ReasonReact.Update(Error)
  | DeckFinished(cards) => ReasonReact.Update(NoMoreCardsToDraw(cards))
  };

let renderParagraph = text => <p> (ReasonReact.stringToElement(text)) </p>;

let renderLoading = () => renderParagraph("Loading...");

let renderError = () =>
  renderParagraph("There was an error. Please refresh and try again!");

let renderCards = cards => {
  let cardElements =
    List.map(c => <CardContainer code=c.code imageSource=c.image />, cards)
    |> Array.of_list
    |> ReasonReact.arrayToElement;
  <div className="App card-list"> cardElements </div>;
};

/* the disableButton parameter is labelled for no particular reason,
   just style */
let renderButtonAndCards = (deck, send, ~disabledButton) =>
  <div className="App">
    <button
      className="App main-action"
      disabled=(Js.Boolean.to_js_boolean(disabledButton))
      onClick=(_ev => send(DrawCards(deck)))>
      (ReasonReact.stringToElement("Draw " ++ drawQuantity(deck)))
    </button>
    (renderCards(deck.cards))
  </div>;

let render = self =>
  ReasonReact.(
    <div className="App">
      (
        switch self.state {
        | CreatingDeck => renderLoading()
        | WaitingForUser(deck) =>
          renderButtonAndCards(deck, self.send, ~disabledButton=false)
        | DrawingCards(deck) =>
          renderButtonAndCards(deck, self.send, ~disabledButton=true)
        | NoMoreCardsToDraw(cards) => renderCards(cards)
        | Error => renderError()
        }
      )
    </div>
  );

let component = ReasonReact.reducerComponent("App");

let make = _self => {
  ...component,
  reducer,
  render,
  initialState: () => CreatingDeck,
  didMount: self => {
    self.send(CreateDeck);
    ReasonReact.NoUpdate;
  }
};

/* wrap it so we don't need to change index.js: */
let default = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));