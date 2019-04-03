let contains = (s1, s2) =>
  try (
    {
      let len = String.length(s2);
      for (i in 0 to String.length(s1) - len) {
        if (String.sub(s1, i, len) == s2) {
          raise(Exit);
        };
      };
      false;
    }
  ) {
  | Exit => true
  };

module App = {
  type pokemonMeta = {
    name: string,
    url: string,
  };
  type state = {
    searchText: string,
    pokemons: list(pokemonMeta),
    pokemonsFiltered: list(pokemonMeta),
  };
  type action =
    | OnSearch(string)
    | FilterUsing(string);

  let component = ReasonReact.reducerComponent("App");
  let make = _children => {
    ...component,
    initialState: () => {
      searchText: "",
      pokemons: [
        {name: "bulbasaur", url: "https://pokeapi.co/api/v2/pokemon/1/"},
        {name: "ivysaur", url: "https://pokeapi.co/api/v2/pokemon/2/"},
        {name: "venusaur", url: "https://pokeapi.co/api/v2/pokemon/3/"},
        {name: "charmander", url: "https://pokeapi.co/api/v2/pokemon/4/"},
        {name: "charmeleon", url: "https://pokeapi.co/api/v2/pokemon/5/"},
        {name: "charizard", url: "https://pokeapi.co/api/v2/pokemon/6/"},
      ],
      pokemonsFiltered: [],
    },
    didMount: self => self.send(FilterUsing("")),
    reducer: (action, state) =>
      switch (action) {
      | OnSearch(searchText) => ReasonReact.Update({...state, searchText})
      | FilterUsing(searchText) =>
        let pokemonsFiltered =
          List.filter(
            pokemon => contains(pokemon.name, searchText),
            state.pokemons,
          );
        ReasonReact.Update({...state, pokemonsFiltered, searchText});
      },
    render: self => {
      let callback = searchText => {
        self.send(OnSearch(searchText));
        self.send(FilterUsing(searchText));
      };
      let pokeList =
        List.mapi(
          (i, pokemon) =>
            <PokemonCard
              key={string_of_int(i)}
              name={pokemon.name}
              url={pokemon.url}
            />,
          self.state.pokemonsFiltered,
        );
      <div className="main-app">
        <SearchBar
          searchText={self.state.searchText}
          onSearch={searchText => callback(searchText)}
        />
        <div className="pokemon-list">
          <div className="row">
            {ReasonReact.array(Array.of_list(pokeList))}
          </div>
        </div>
      </div>;
    },
  };
};

ReactDOMRe.renderToElementWithId(<App />, "app");
