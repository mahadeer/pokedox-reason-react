/* Call window object function to cache images */
[@bs.val] external toDataURL: (string, string => unit) => unit = "toDataURL";

type state = {
  pokeId: string,
  shiny: bool,
  imageStr: string,
  shinyBase64: string,
  base64: string,
};

type action =
  | ChangeImage
  | SetImageBase64(bool, string);

let component = ReasonReact.reducerComponent("PokemonCard");

let make = (~name, ~url, _children) => {
  /* src=self.state.imageUrl */
  let getPokeId = text =>
    String.make(1, text.[String.rindex(text, '/') - 1]);
  {
    ...component,
    didMount: self => {
      let pokeId = getPokeId(url);
      /* Cache normal image */
      toDataURL(
        {j|https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/$pokeId.png|j},
        base64Str =>
        self.send(SetImageBase64(false, base64Str))
      );
      /* Cache shiny normal image */
      toDataURL(
        {j|https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/shiny/$pokeId.png|j},
        base64Str =>
        self.send(SetImageBase64(true, base64Str))
      );
      let intervalId =
        Js.Global.setInterval(() => self.send(ChangeImage), 1000);
      self.onUnmount(() => Js.Global.clearInterval(intervalId));
    },
    initialState: () => {
      pokeId: getPokeId(url),
      shiny: false,
      imageStr: "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/0.png",
      shinyBase64: "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/0.png",
      base64: "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/0.png",
    },
    reducer: (action, state) =>
      switch (action) {
      | ChangeImage =>
        if (state.shiny) {
          ReasonReact.Update({
            ...state,
            shiny: !state.shiny,
            imageStr: state.shinyBase64,
          });
        } else {
          ReasonReact.Update({
            ...state,
            shiny: !state.shiny,
            imageStr: state.base64,
          });
        }
      | SetImageBase64(isShiny, resultStr) =>
        if (isShiny) {
          ReasonReact.Update({...state, shinyBase64: resultStr});
        } else {
          ReasonReact.Update({...state, base64: resultStr});
        }
      },
    render: self =>
      <div className="col-sm-6 mb-3">
        <div className="card">
          <div className="row no-gutters">
            <div className="col-md-4">
              <img src={self.state.imageStr} className="card-img" alt=name />
            </div>
            <div className="col-md-8">
              <div className="card-body">
                <h5 className="card-title"> {ReasonReact.string(name)} </h5>
                <p className="card-text"> {ReasonReact.string(url)} </p>
              </div>
            </div>
          </div>
        </div>
      </div>,
  };
};
