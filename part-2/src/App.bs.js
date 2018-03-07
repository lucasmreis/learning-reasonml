// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

import * as List                     from "bs-platform/lib/es6/list.js";
import * as $$Array                  from "bs-platform/lib/es6/array.js";
import * as Block                    from "bs-platform/lib/es6/block.js";
import * as Curry                    from "bs-platform/lib/es6/curry.js";
import * as React                    from "react";
import * as Js_boolean               from "bs-platform/lib/es6/js_boolean.js";
import * as Pervasives               from "bs-platform/lib/es6/pervasives.js";
import * as Json_decode              from "@glennsl/bs-json/src/Json_decode.bs.js";
import * as ReasonReact              from "reason-react/src/ReasonReact.js";
import * as CardContainer$SimpleDeck from "./components/Card/CardContainer.bs.js";

import './App.css'
;

function drawQuantity(deck) {
  return Pervasives.min(deck[/* remaining */1], 3).toString();
}

function decodeCreatedDeck(json) {
  return /* record */[
          /* deckId */Json_decode.field("deck_id", Json_decode.string, json),
          /* remaining */Json_decode.field("remaining", Json_decode.$$int, json),
          /* cards : [] */0
        ];
}

function decodeCard(json) {
  return /* record */[
          /* image */Json_decode.field("image", Json_decode.string, json),
          /* code */Json_decode.field("code", Json_decode.string, json)
        ];
}

function decodeDeck(json) {
  return /* record */[
          /* deckId */Json_decode.field("deck_id", Json_decode.string, json),
          /* remaining */Json_decode.field("remaining", Json_decode.$$int, json),
          /* cards */Json_decode.field("cards", (function (param) {
                  return Json_decode.list(decodeCard, param);
                }), json)
        ];
}

var component = ReasonReact.reducerComponent("App");

function createDeckSideEffects(send) {
  fetch("https://deckofcardsapi.com/api/deck/new/shuffle/").then((function (prim) {
              return prim.json();
            })).then((function (json) {
            var deck = decodeCreatedDeck(json);
            return Promise.resolve(Curry._1(send, /* DeckCreated */Block.__(0, [deck])));
          })).catch((function () {
          return Promise.resolve(Curry._1(send, /* CreateDeckFailed */1));
        }));
  return /* () */0;
}

function drawCardsSideEffects(stateDeck, send) {
  fetch("https://deckofcardsapi.com/api/deck/" + (stateDeck[/* deckId */0] + ("/draw/?count=" + drawQuantity(stateDeck)))).then((function (prim) {
            return prim.json();
          })).then((function (json) {
          var receivedDeck = decodeDeck(json);
          return Promise.resolve(receivedDeck[/* remaining */1] > 0 ? Curry._1(send, /* CardsDrawn */Block.__(2, [/* record */[
                                /* deckId */receivedDeck[/* deckId */0],
                                /* remaining */receivedDeck[/* remaining */1],
                                /* cards */Pervasives.$at(stateDeck[/* cards */2], receivedDeck[/* cards */2])
                              ]])) : Curry._1(send, /* Finish */Block.__(3, [stateDeck[/* cards */2]])));
        }));
  return /* () */0;
}

function renderButtonAndCards(deck, send, disabledButton) {
  var cards = $$Array.of_list(List.map((function (c) {
              return ReasonReact.element(/* None */0, /* None */0, CardContainer$SimpleDeck.make(c[/* code */1], c[/* image */0], /* array */[]));
            }), deck[/* cards */2]));
  return React.createElement("div", {
              className: "App"
            }, React.createElement("button", {
                  className: "App main-action",
                  disabled: Js_boolean.to_js_boolean(disabledButton),
                  onClick: (function () {
                      return Curry._1(send, /* DrawCards */Block.__(1, [deck]));
                    })
                }, "Draw " + drawQuantity(deck)), React.createElement("div", {
                  className: "App card-list"
                }, cards));
}

function make() {
  var newrecord = component.slice();
  newrecord[/* didMount */4] = (function (self) {
      Curry._1(self[/* send */4], /* CreateDeck */0);
      return /* NoUpdate */0;
    });
  newrecord[/* render */9] = (function (self) {
      var match = self[/* state */2];
      var tmp;
      if (typeof match === "number") {
        tmp = match ? React.createElement("p", undefined, "There was an error. Please refresh and try again!") : React.createElement("p", undefined, "Loading...");
      } else {
        switch (match.tag | 0) {
          case 0 : 
              tmp = renderButtonAndCards(match[0], self[/* send */4], /* false */0);
              break;
          case 1 : 
              tmp = renderButtonAndCards(match[0], self[/* send */4], /* true */1);
              break;
          case 2 : 
              tmp = React.createElement("p", undefined, "Finished");
              break;
          
        }
      }
      return React.createElement("div", {
                  className: "App"
                }, tmp);
    });
  newrecord[/* initialState */10] = (function () {
      return /* CreatingDeck */0;
    });
  newrecord[/* reducer */12] = (function (action, _) {
      if (typeof action === "number") {
        switch (action) {
          case 0 : 
              return /* UpdateWithSideEffects */Block.__(3, [
                        /* CreatingDeck */0,
                        (function (self) {
                            return createDeckSideEffects(self[/* send */4]);
                          })
                      ]);
          case 1 : 
          case 2 : 
              return /* Update */Block.__(0, [/* Error */1]);
          
        }
      } else {
        switch (action.tag | 0) {
          case 1 : 
              var stateDeck = action[0];
              return /* UpdateWithSideEffects */Block.__(3, [
                        /* DrawingCards */Block.__(1, [stateDeck]),
                        (function (self) {
                            return drawCardsSideEffects(stateDeck, self[/* send */4]);
                          })
                      ]);
          case 0 : 
          case 2 : 
              return /* Update */Block.__(0, [/* WaitingForUser */Block.__(0, [action[0]])]);
          case 3 : 
              return /* Update */Block.__(0, [/* Finished */Block.__(2, [action[0]])]);
          
        }
      }
    });
  return newrecord;
}

var $$default = ReasonReact.wrapReasonForJs(component, (function () {
        return make(/* array */[]);
      }));

var number_of_cards_per_draw = 3;

export {
  number_of_cards_per_draw ,
  drawQuantity             ,
  decodeCreatedDeck        ,
  decodeCard               ,
  decodeDeck               ,
  component                ,
  createDeckSideEffects    ,
  drawCardsSideEffects     ,
  renderButtonAndCards     ,
  make                     ,
  $$default                ,
  $$default                  as default,
  
}
/*  Not a pure module */
