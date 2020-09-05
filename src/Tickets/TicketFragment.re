module User=[%graphql
  {|
  fragment User on User @bsRecord {
    avatarUrl
    fullName
  }
|}
];

module Ticket =[%graphql
  {|

  fragment Ticket on Ticket @bsRecord {
    assignee {
      ... on User {
        ...User
      }
      ... on WorkingGroup @bsRecord {
        id
        name
      }
    }
    id
    subject
    status
    lastUpdated
  }
|};
  {inline: true}
];
