[%graphql
  {|
    # Check it out! This will be exposed as a module called TodoItem!
    fragment TodoItem on TodoItem {
        id
        text
        completed
    }

    query Fragments {
      allTodos {
        ...TodoItem
      }
    }
  |};
  {inline: true}
];

[%graphql
  {|
  fragment UserAvatar on User {
    avatarUrl
    fullName
  }
  query User ($id:ID!){
      userById(id:$id) {
        ...UserAvatar
      }
    }
|};
  {inline: true}
];

// open Types;

[%graphql
  {|
  fragment User on User @bsRecord {
    avatarUrl
    fullName
  }
|};
  {inline: true}
];
