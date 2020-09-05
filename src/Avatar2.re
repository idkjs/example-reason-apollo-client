open Types;

module Fragment = [%graphql
  {|
  fragment User on User @bsRecord {
    avatarUrl
    fullName
  }
|}
];

[@react.component]
let make = (~user) => {
  <>
    {switch (user.avatarUrl) {
     | Some(avatarUrl) => <img src=avatarUrl className="mr-2" alt="image" />
     | None => React.null
     }}
    {React.string(user.fullName)}
  </>;
};
