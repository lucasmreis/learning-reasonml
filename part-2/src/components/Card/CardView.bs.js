// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

import * as React                         from "react";
import * as ReasonReact                   from "reason-react/src/ReasonReact.js";
import * as ParseAndRenderCard$SimpleDeck from "./ParseAndRenderCard.bs.js";

import './Card.css'
;

var component = ReasonReact.statelessComponent("CardView");

function make(code, imageSource, flipped, onClick, _) {
  var newrecord = component.slice();
  newrecord[/* render */9] = (function () {
      return React.createElement("div", {
                  className: flipped !== 0 ? "Card flipped" : "Card",
                  onClick: onClick
                }, React.createElement("div", {
                      className: "Card front"
                    }, React.createElement("img", {
                          alt: ParseAndRenderCard$SimpleDeck.parseAndRender(code),
                          src: imageSource
                        })), React.createElement("div", {
                      className: "Card back"
                    }));
    });
  return newrecord;
}

export {
  component ,
  make      ,
  
}
/*  Not a pure module */
