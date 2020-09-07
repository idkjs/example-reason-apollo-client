module MainConfig = [%graphql
  {|
  query {
    siteStatistics {
      ...Statistics
    }
  }
|}
];
// module Query = [%graphql
//   {|
//   query {
//     siteStatistics {
//       ...Statistics
//     }
//   }
// |}
// ];

[@react.component]
let make = () => {
  let simple = MainConfig.use();
  // let query = Query.use();
  // let x = Query.makeDefaultVariables();
  <div className="main-panel">
    <div className="content-wrapper">
      <Header />
      {switch (simple) {
       | {data: Some(data)} =>
         <SiteStatistics statistics={data.siteStatistics} />
       | {error} =>
         <>
           "Error loading data"->React.string
           {switch (error) {
            | Some(error) => React.string(": " ++ error.message)
            | None => React.null
            }}
         </>
       }}
      <div className="row">
        <div className="col-7 grid-margin"> <RecentTickets /> </div>
        <div className="col-5 grid-margin"> <TodoList /> </div>
        <div className="col-5 grid-margin">
          // <TodoList2 query={x} />
        </div>
      </div>
    </div>
  </div>;
};
