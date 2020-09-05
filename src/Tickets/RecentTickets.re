open Types;
module QueryResult = ApolloClient.Types.QueryResult;

// module User = Fragments.User;
[%graphql
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
|}
];
// module Ticket = TicketFragment.Ticket;
module TicketsQuery = [%graphql
  {|
  query($offset: Int!, $limit: Int!) {
    tickets(offset: $offset, limit: $limit) {
      __typename
      hasNextPage
      results {
        ...Ticket
      }
    }
  }
|}
];
external toTickets: Js.Json.t => TicketsQuery.t = "%identity";
external toJson: TicketsQuery.t => Js.Json.t = "%identity";

[@react.component]
let make = (~query as queryRef) => {
  let queryResult = TicketsQuery.use(queryRef);
  // let ReasonRelay.{data, hasNext, isLoadingNext, loadNext} =
  //   Fragment.usePagination(queryRef);
  <div>
    {switch (queryResult) {
           | {loading: true, data: None} => <p> "Loading"->React.string </p>
     | {loading, data: Some({tickets}), error} =>
  <div className="card">
    <div className="card-body">
      <h4 className="card-title"> {React.string("Recent Tickets")} </h4>
      <div className="table-responsive">
        <table className="table">
          <thead>
            <tr>
              <th> {React.string("Assignee")} </th>
              <th> {React.string("Subject")} </th>
              <th> {React.string("Status")} </th>
              <th> {React.string("Last Update")} </th>
              <th> {React.string("Tracking ID")} </th>
            </tr>
          </thead>
          <tbody>
            {tickets.ticketsConnection
             ->QueryResult.getConnectionNodes
             ->Belt.Array.map(ticket =>
                 <SingleTicket key={ticket.id} ticket={ticket.fragmentRefs} />
               )
             ->React.array}
          </tbody>
        </table>
        {hasNext
           ? <button
               className="btn btn-gradient-primary font-weight-bold"
               id="add-task"
               onClick={_ => loadNext(~count=2, ()) |> ignore}
               disabled=isLoadingNext>
               {React.string("More")}
             </button>
           : React.null}
      </div>
    </div>
  </div>
    }}
    </div>;
};
