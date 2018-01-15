type t =
  | Flipped
  | NotFlipped;

let flip = card =>
  switch card {
  | Flipped => NotFlipped
  | NotFlipped => Flipped
  };