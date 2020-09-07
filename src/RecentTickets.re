module QueryResult = ApolloClient.Types.QueryResult;
module FetchResult = ApolloClient.Types.FetchResult;
module Ticket = TicketFragment;

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

// let mergeTickets = (prevData, options): Js.Json.t => {
//   switch (options->QueryResult.fetchMore) {
//   | Some(newData) =>
//     let newResult = toTickets(newData)##tickets;
//     let prevResult = toTickets(prevData)##tickets;

//     let mergedResult = {
//       "tickets": {
//         "__typename": newResult##__typename,
//         "hasNextPage": newResult##hasNextPage,
//         "results": prevResult##results->Belt.Array.concat(newResult##results),
//       },
//     };
//     toJson(mergedResult);
//   | None => prevData
//   };
// };

let limit = 3;

[@react.component]
let make = () => {
  let queryResult = TicketsQuery.use({offset: 0, limit});
  //   let (simpleResult, fullResult) = TicketsQuery.use({offset: 0, limit});
  let isFetchingMore =
    queryResult.networkStatus
    == ApolloClient__React_Types.NetworkStatus.Loading;

  let (hasNextPage, offset) =
    switch (queryResult) {
    | {data: Some({tickets: {hasNextPage, results}})} => (
        hasNextPage,
        results->Belt.Array.length,
      )
    | _ => (false, 0)
    };

  let handleLoadMore = () => {
    QueryResult.fetchMore(
      ~variables=TicketsQuery.makeVariables(~offset, ~limit, ()),
      //   ~updateQuery=mergeTickets,
    )
    |> ignore;
  };

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
            </tr>
          </thead>
          {switch (queryResult) {
           | {loading: true, data: None} => React.null
           | {data: Some({tickets: {hasNextPage, results}})} =>
             <tbody>
               {results->Belt.Array.map(ticket =>
                  <TicketUi key={ticket.id} ticket />
                )
                |> React.array}
             </tbody>
           | {error: Some(error)} =>
             <div> {ReasonReact.string(error.message)} </div>
           | {error: None, data: None, loading: false} =>
             React.string("Not asked")
           }}
        </table>
        {hasNextPage
           ? <button
               className="btn btn-gradient-primary font-weight-bold"
               id="add-task"
               onClick={_ => handleLoadMore()}
               disabled=isFetchingMore>
               {React.string("More")}
             </button>
           : React.null}
      </div>
    </div>
  </div>;
};
