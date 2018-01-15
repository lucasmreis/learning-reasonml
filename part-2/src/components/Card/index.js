import React from "react";
import "./Card.css";
import { parseAndRender } from "./ParseAndRenderCard.bs";

class Card extends React.Component {
  state = {
    flipped: true
  };

  flip = () => {
    this.setState({ flipped: !this.state.flipped });
  };

  render() {
    const { code, imageSource } = this.props;
    const flippedClass = this.state.flipped ? "Card flipped" : "Card";
    return (
      <div className={flippedClass} onClick={this.flip}>
        <div className="Card front">
          <img alt={parseAndRender(code)} src={imageSource} />
        </div>
        <div className="Card back" />
      </div>
    );
  }
}

export { Card };
