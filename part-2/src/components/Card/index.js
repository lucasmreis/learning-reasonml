import React from "react";
import "./Card.css";
import CardView from "./CardView.bs";

class Card extends React.Component {
  state = { flipped: true };

  flip = () => {
    this.setState({ flipped: !this.state.flipped });
  };

  render() {
    const { code, imageSource } = this.props;
    return (
      <CardView
        code={code}
        imageSource={imageSource}
        flipped={this.state.flipped}
        onClick={this.flip}
      />
    );
  }
}

export { Card };
