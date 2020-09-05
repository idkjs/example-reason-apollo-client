module QueryResult = ApolloClient.Types.QueryResult;

// The TodosQuery below is going to look for a module of the same name to define the fragment
module UserAvatar = AvatarFragments.UserAvatar;

module User = [%graphql
  {|
    query UserQuery($id:ID!) {
      user: userById(id: $id) {
        # This references the TodoItem fragment definition module above!
        ...UserAvatar
      }
    }
  |}
];

[@react.component]
let make = (~user as userRef: string) => {
  let user = User.use({id: userRef});

  switch (User.use({id: userRef})) {
  | {loading: true} => "Loading..."->React.string
  // | {error: Some(_error)} => "Error loading data"->React.string
  // | {data: Some({user: Some({avatarUrl, fullName})}), error} =>
  | {data: Some({user})} =>

    Js.log2("USER", user);
    let avatarUrl =
      user
      ->Belt.Option.map(avatarUrl =>
          <img src={avatarUrl->Obj.magic} className="mr-2" alt="image" />
        )
      ->Belt.Option.getWithDefault(React.null);

    let fullName =
      user
      ->Belt.Option.map(fullName => {React.string(fullName->Obj.magic)})
      ->Belt.Option.getWithDefault(React.null);

    <> avatarUrl fullName </>;
  | _ => React.null
  };
};
