import React, { Component } from "react";
import "./App.css";

import { Card } from "./components/Card";

const NUMBER_OF_CARDS_PER_DRAW = 3;

// { status: 'CREATING_DECK' }
// { status: 'WAITING_FOR_USER', deckId, cards: [{ image, code }], remaining }
// { status: 'DRAWING_CARDS', deckId, cards, remaining }
// { status: 'FINISHED', cards }
// { status: 'ERROR' }

class App extends Component {
  state = { status: "CREATING_DECK" };

  componentDidMount() {
    fetch("https://deckofcardsapi.com/api/deck/new/shuffle/")
      .then(res => res.json())
      .then(
        deck =>
          deck.success
            ? this.setState({
                status: "WAITING_FOR_USER",
                deckId: deck.deck_id,
                cards: [],
                remaining: deck.remaining
              })
            : this.setState({ status: "ERROR" })
      );
  }

  render() {
    return (
      <div className="App">
        {this.renderMainAction()}
        {this.renderCards()}
      </div>
    );
  }

  renderMainAction() {
    const { status } = this.state;
    const remaining = drawQuantity(this.state);
    if (status === "WAITING_FOR_USER") {
      return (
        <button className="App main-action" onClick={this.draw}>
          Draw {remaining}
        </button>
      );
    } else if (status === "DRAWING_CARDS") {
      return (
        <button className="App main-action" disabled onClick={this.draw}>
          Draw {remaining}
        </button>
      );
    } else {
      return null;
    }
  }

  renderCards() {
    const { status, cards } = this.state;
    if (status === "WAITING_FOR_USER" || status === "DRAWING_CARDS") {
      return (
        <div className="App card-list">
          {cards.map(c => (
            <Card code={c.code} key={c.code} imageSource={c.image} />
          ))}
        </div>
      );
    }
  }

  //
  // ACTIONS
  //

  draw = async () => {
    const { deckId, cards } = this.state;
    const qty = drawQuantity(this.state);
    const url = `https://deckofcardsapi.com/api/deck/${deckId}/draw/?count=${qty}`;
    await this.setState({ status: "DRAWING_CARDS" });
    fetch(url)
      .then(res => res.json())
      .then(
        deck =>
          deck.success
            ? this.setState({
                status: deck.remaining ? "WAITING_FOR_USER" : "FINISHED",
                deckId: deck.deck_id,
                cards: cards.concat(deck.cards),
                remaining: deck.remaining
              })
            : this.setState({ status: "ERROR" })
      );
  };
}

//
// SELECTORS
//

const drawQuantity = state =>
  Math.min(state.remaining, NUMBER_OF_CARDS_PER_DRAW);

export default App;
