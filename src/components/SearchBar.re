let component = ReasonReact.statelessComponent("SearchBar");

let make = (~searchText, ~onSearch, _children) => {
  ...component,
  render: _self =>
    <div className="mx-auto mt-5 mb-3">
      <input
        className="form-control"
        placeholder="Search..."
        defaultValue=searchText
        onChange={
          event => onSearch(ReactEvent.Form.target(event)##value)
        }
      />
    </div>,
};
