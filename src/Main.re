
module QueryResult = ApolloClient.Types.QueryResult;

module Statistics = SiteStatistics.Fragment;
module Statistics = MainFragments.Statistics;

[%graphql
  {|
  query MainQuery {
    siteStatistics {
      ...Statistics
    }
  }
|}
];

[@react.component]
let make = () => {
 let (simple, _) = MainQuery.makeSimple();

//   <div className="main-panel">
//     <div className="content-wrapper">
//       <Header />
//       <TopCardsDisplayer siteStatistics={query.siteStatistics.fragmentRefs} />
//       <div className="row">
//         <div className="col-8 grid-margin">
//           <RecentTickets query={query.fragmentRefs} />
//         </div>
//         <div className="col-4 grid-margin">
//           <TodoList query={query.fragmentRefs} />
//         </div>
//       </div>
//     </div>
//   </div>;
  React.null
};
