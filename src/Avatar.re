[@react.component]
let make = (~user:User.t) => {
  <>
    {switch (user.avatarUrl) {
     | Some(avatarUrl) => <img src=avatarUrl className="mr-2" alt="image" />
     | None => React.null
     }}
    {React.string(user.fullName)}
  </>;
};
