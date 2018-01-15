// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

import * as Block                           from "bs-platform/lib/es6/block.js";
import * as Curry                           from "bs-platform/lib/es6/curry.js";
import * as ReasonReact                     from "reason-react/src/ReasonReact.js";
import * as CardView$LearningReasonmlPart2  from "./CardView.bs.js";
import * as CardState$LearningReasonmlPart2 from "./CardState.bs.js";

var component = ReasonReact.reducerComponent("CardContainer");

function make(code, imageSource, _) {
  var newrecord = component.slice();
  newrecord[/* render */9] = (function (self) {
      var flipped = self[/* state */2];
      return ReasonReact.element(/* None */0, /* None */0, CardView$LearningReasonmlPart2.make(code, imageSource, flipped, (function () {
                        return Curry._1(self[/* send */4], /* Flip */0);
                      }), /* array */[]));
    });
  newrecord[/* initialState */10] = (function () {
      return /* Flipped */0;
    });
  newrecord[/* reducer */12] = (function (_, state) {
      return /* Update */Block.__(0, [CardState$LearningReasonmlPart2.flip(state)]);
    });
  return newrecord;
}

var $$default = ReasonReact.wrapReasonForJs(component, (function (jsProps) {
        return make(jsProps.code, jsProps.imageSource, /* array */[]);
      }));

export {
  component ,
  make      ,
  $$default ,
  $$default   as default,
  
}
/* component Not a pure module */