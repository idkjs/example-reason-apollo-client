[@react.component]
let make = (~status) =>
  switch (status) {
  | `Done =>
    <label className="badge badge-gradient-success">
      {React.string("DONE")}
    </label>
  | `Progress =>
    <label className="badge badge-gradient-warning">
      {React.string("PROGRESS")}
    </label>
  | `OnHold =>
    <label className="badge badge-gradient-info">
      {React.string("ON HOLD")}
    </label>
  | `Rejected =>
    <label className="badge badge-gradient-danger">
      {React.string("REJECTED")}
    </label>
  | _ => React.null
  };
