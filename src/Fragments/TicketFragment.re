[%graphql
  {|
  fragment Ticket on Ticket {
    assignee {
      ... on User {
        ...User
      }
      ... on WorkingGroup {
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