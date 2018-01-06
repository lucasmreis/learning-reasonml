open Js.String;

let parseAndRenderSuit = suitStr =>
  switch suitStr {
  | "H" => Some("Hearts")
  | "D" => Some("Diamonds")
  | "C" => Some("Clubs")
  | "S" => Some("Spades")
  | _ => None
  };

let parseAndRenderValue = valueStr =>
  switch valueStr {
  | "2" => Some("Two")
  | "3" => Some("Three")
  | "4" => Some("Four")
  | "5" => Some("Five")
  | "6" => Some("Six")
  | "7" => Some("Seven")
  | "8" => Some("Eight")
  | "9" => Some("Nine")
  | "10" => Some("Ten")
  | "J" => Some("Jack")
  | "Q" => Some("Queen")
  | "K" => Some("King")
  | "A" => Some("Ace")
  | _ => None
  };

let parseAndRenderCard = cardStr => {
  /* Separating the input string: */
  let length = length(cardStr);
  let suitStr = sliceToEnd(~from=length - 1, cardStr);
  let valueStr = slice(~from=0, ~to_=length - 1, cardStr);
  /* Parsing and rendering the strings with our functions: */
  let renderedSuit = parseAndRenderSuit(suitStr);
  let renderedValue = parseAndRenderValue(valueStr);
  /* If inputs were valid, print the card: */
  switch (renderedValue, renderedSuit) {
  | (Some(value), Some(suit)) => value ++ " of " ++ suit
  | _ => "-- unknown card --"
  };
};

"QC" |> parseAndRenderCard |> Js.log;