let str = React.string;

let decodeDate = date =>
  date
  ->Belt.Option.flatMap(Js.Json.decodeString)
  ->Belt.Option.map(Js.Date.fromString);

let decodedDate = date => {
  let date = date->decodeDate;
  switch (date) {
  | Some(date) => date->Js.Date.toLocaleString
  | None => "-"
  };
};
module Fragment = [%graphql
  {|
  fragment Ticket on Ticket  {
    assignee {
      ... on User {
        ...User
      }
      ... on WorkingGroup  {
        id
        name
      }
    }
    id
    subject
    status
    lastUpdated
  }
|}
];

[@react.component]
let make = (~ticket: TicketFragment.t) => {
  <tr>
    <td>
      {switch (ticket.assignee) {
       | Some(assignee) =>
         switch (assignee) {
         | `User(user) => <Avatar user />
         | `FutureAddedValue(_) => React.null
         | `WorkingGroup(workingGroup) =>
           <strong> {str(workingGroup.name)} </strong>
         }
       | None => <em> {str("Unassigned")} </em>
       }}
    </td>
    <td> {str(ticket.subject)} </td>
    <td> <TicketStatusBadge status={ticket.status} /> </td>
    <td> {str(ticket.lastUpdated->decodedDate)} </td>
  </tr>;
};
