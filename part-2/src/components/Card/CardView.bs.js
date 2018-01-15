// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

import * as React                                    from "react";
import * as ReasonReact                              from "reason-react/src/ReasonReact.js";
import * as ParseAndRenderCard$LearningReasonmlPart2 from "./ParseAndRenderCard.bs.js";

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
                          alt: ParseAndRenderCard$LearningReasonmlPart2.parseAndRender(code),
                          src: imageSource
                        })), React.createElement("div", {
                      className: "Card back"
                    }));
    });
  return newrecord;
}

var $$default = ReasonReact.wrapReasonForJs(component, (function (jsProps) {
        return make(jsProps.code, jsProps.imageSource, +jsProps.flipped, jsProps.onClick, /* array */[]);
      }));

export {
  component ,
  make      ,
  $$default ,
  $$default   as default,
  
}
/* component Not a pure module */
