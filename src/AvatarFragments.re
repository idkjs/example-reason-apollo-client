[%graphql
  {|
    # Check it out! This will be exposed as a module called TodoItem!
    fragment UserAvatar on User {
      avatarUrl
      fullName
    }

    query Fragments($id:ID!){
      userById(id:$id) {
        ...UserAvatar
      }
    }
  |};
  {inline: true}
];
