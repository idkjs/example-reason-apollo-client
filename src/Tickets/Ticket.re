open Types;
let str = React.string;

let decodeDate = date =>
  date
  ->Belt.Option.flatMap(Js.Json.decodeString)
  ->Belt.Option.map(Js.Date.fromString);

module Fragment = [%graphql
  {|
  fragment Ticket on Ticket @bsRecord {
    assignee {
      ... on User {
        ...Avatar.Fragment.User
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
|}
];

[@react.component]
let make = (~ticket) => {
  <tr>
    <td>
      {switch (ticket.assignee) {
       | Some(assignee) =>
         switch (assignee) {
         | `User(user) => <Avatar user />
         | `WorkingGroup(workingGroup) =>
           <strong> {str(workingGroup.name)} </strong>
         }
       | None => <em> {str("Unassigned")} </em>
       }}
    </td>
    <td> {str(ticket.subject)} </td>
    <td> <TicketStatusBadge status={ticket.status} /> </td>
    <td>
      {ticket.lastUpdated
       ->Belt.Option.mapWithDefault("-", Js.Date.toLocaleString)
       ->str}
    </td>
  </tr>;
};
